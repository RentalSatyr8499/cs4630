#include "ipc.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


void usage(char *name) {
    fprintf(stderr, "Usage: %s in INFILE1 out OUTFILE1 "
                    "in INFILE2 out OUTFILE2 ...\n", name);
    fprintf(stderr, "   or: %s literal STRING\n", name);
    exit(1);
}


int main(int argc, char **argv) {
    start_worker_from_coordinator("./worker");

    int i = 1;
    while (i < argc) {
        if (0 == strcmp("in", argv[i])) {
            if (argc <= i+3) usage(argv[0]);
            char *in_filename = argv[i+1];
            if (0 != strcmp("out", argv[i+2])) usage(argv[0]);
            char *out_filename = argv[i+3];
            i += 4;
            if (0 == access(out_filename, F_OK)) {
                fprintf(stderr, "Refusing to overwrite existing '%s'\n", out_filename);
                continue;
            }
            FILE *in_handle = fopen(in_filename, "r");
            if (in_handle == NULL) {
                fprintf(stderr, "Error opening %s: %s\n", in_filename, strerror(errno));
                continue;
            }
            FILE *out_handle = fopen(out_filename, "w");
            if (out_handle == NULL) {
                fprintf(stderr, "Error opening %s: %s\n", out_filename, strerror(errno));
                continue;
            }
            char *input = NULL;
            size_t size = 0;
            if (-1 == getdelim(&input, &size, '\0', in_handle)) {
                fprintf(stderr, "Error reading %s: %s\n", in_filename, strerror(errno));
            }

            send_message_to_worker((struct message){
                strlen(input)+1, // size
                input            // data
            });
            char *output = recv_message_from_worker().data;

            free(input);
            fputs(output, out_handle);
            free(output);
            fclose(in_handle);
            fclose(out_handle);
        } else if (0 == strcmp("literal", argv[i])) {
            if (argc <= i+1) usage(argv[0]);

            send_message_to_worker((struct message){
                strlen(argv[i+1])+1, // size
                argv[i+1]            // data
            });
            printf("%s\n", recv_message_from_worker().data);

            i += 2;
        } else {
            usage(argv[0]);
        }
    }
    send_message_to_worker((struct message){0, ""});
    wait_for_worker_to_exit();
}
