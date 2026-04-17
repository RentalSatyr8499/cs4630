#ifndef IPC_H_
#define IPC_H_

#include <sys/types.h>

/* See the assignment writeup for documentation about the structs/functions in this file */

struct message {
    size_t size;
    char *data;
};

void start_worker_from_coordinator(const char *command);
void send_message_to_worker(struct message message);
struct message recv_message_from_worker(void);
void send_message_to_coordinator(struct message message);
struct message recv_message_from_coordinator(void);
void wait_for_worker_to_exit();

#endif
