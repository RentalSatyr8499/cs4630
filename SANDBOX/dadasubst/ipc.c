#define _GNU_SOURCE
#include "ipc.h"
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define CHECK(x) \
    do { \
        errno = 0; \
        if (!(x)) { \
            if (errno != 0) { \
                fprintf(stderr, \
                    "%s:%d: CHECK(%s) failed: system error=%s (%s)\n", \
                    __FILE__, __LINE__, \
                    # x, strerror(errno), strerrorname_np(errno) \
                ); \
            } else { \
                fprintf(stderr, \
                    "%s:%d: CHECK(%s) failed (short read/write?)\n", \
                    __FILE__, __LINE__, \
                    # x \
                ); \
            } \
            abort(); \
        } \
    } while (0)

static bool read_full(int fd, void *buffer, size_t size) {
    ssize_t offset = 0;
    do {
        errno = 0;
        ssize_t current_count = read(fd, buffer + offset, size - offset);
        if (current_count <= 0) {
            return false;
        }
        offset += current_count;
    } while (offset != size);
    return true;
}

static bool write_full(int fd, const void *buffer, size_t size) {
    ssize_t offset = 0;
    do {
        errno = 0;
        ssize_t current_count = write(fd, buffer + offset, size - offset);
        if (current_count <= 0) {
            return false;
        }
        offset += current_count;
    } while (offset != size);
    return true;
}

static struct message read_message(int fd) {
    struct message m;
    CHECK(read_full(fd, &m.size, sizeof(m.size)));
    if (m.size < 0 || m.size > 1000000) {
        fprintf(stderr, "bad size %ld received\n", (long) m.size);
        abort();
    }
    m.data = calloc(1, m.size + 1);
    if (m.size > 0) {
        CHECK(read_full(fd, m.data, m.size));
    }
    return m;
}

static void write_message(int fd, struct message m) {
    CHECK(write_full(fd, &m.size, sizeof(m.size)));
    if (m.size > 0) {
        CHECK(write_full(fd, m.data, m.size));
    }
}

static int coordinator_out_pipe_fd = -1;
static int coordinator_in_pipe_fd = -1;
static int worker_out_pipe_fd = -1;
static int worker_in_pipe_fd = -1;
static pid_t other_pid;

static bool setup_worker() {
    if (worker_out_pipe_fd == -1) {
        char *out_fd_str = getenv("_IPC_WORKER_OUT");
        char *in_fd_str = getenv("_IPC_WORKER_IN");
        if (out_fd_str != NULL && in_fd_str != NULL) {
            worker_out_pipe_fd = atoi(out_fd_str);
            worker_in_pipe_fd = atoi(in_fd_str);
        }
    }
    return worker_out_pipe_fd != -1;
}

void wait_for_worker_to_exit() {
    CHECK(coordinator_out_pipe_fd != -1 /* worker is running */);
    close(coordinator_out_pipe_fd);
    close(coordinator_in_pipe_fd);
    waitpid(other_pid, NULL, 0);
}

void start_worker_from_coordinator(const char *command) {
    int coordinator_out_pipe_fds[2];
    int coordinator_in_pipe_fds[2];
    CHECK(pipe(coordinator_out_pipe_fds) == 0);
    CHECK(pipe(coordinator_in_pipe_fds) == 0);
    other_pid = fork();
    if (other_pid < 0) {
        CHECK(0 /* fork returned -1 */);
    } else if (other_pid == 0) {
        char temp_number[10];
        snprintf(temp_number, sizeof temp_number, "%d", coordinator_out_pipe_fds[0]);
        setenv("_IPC_WORKER_IN", temp_number, 1);
        snprintf(temp_number, sizeof temp_number, "%d", coordinator_in_pipe_fds[1]);
        setenv("_IPC_WORKER_OUT", temp_number, 1);
        close(coordinator_out_pipe_fds[1]);
        close(coordinator_in_pipe_fds[0]);
        execl("/bin/sh", "sh", "-c", command, NULL);
        CHECK(0 /* reached code after execl */);
    }
    close(coordinator_out_pipe_fds[0]); close(coordinator_in_pipe_fds[1]);
    coordinator_out_pipe_fd = coordinator_out_pipe_fds[1];
    coordinator_in_pipe_fd = coordinator_in_pipe_fds[0];
}

void send_message_to_worker(struct message m) {
    CHECK(coordinator_out_pipe_fd != -1 /* start_worker_from_coordinator called */);
    write_message(coordinator_out_pipe_fd, m);
}

void send_message_to_coordinator(struct message m) {
    CHECK(setup_worker());
    write_message(worker_out_pipe_fd, m);
}

struct message recv_message_from_worker() {
    CHECK(coordinator_in_pipe_fd != -1 /* start_worker_from_coordinator called */);
    return read_message(coordinator_in_pipe_fd);
}

struct message recv_message_from_coordinator() {
    CHECK(setup_worker());
    return read_message(worker_in_pipe_fd);
}
