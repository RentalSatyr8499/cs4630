#include "ipc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char *dadasubst(char*); 

void main(){
    struct message *result = malloc(sizeof(struct message));

    while(1){
        struct message msg = recv_message_from_coordinator();
        if (msg.size == 0) break;
        printf("worker gota message!\n");

        result->data = dadasubst(msg.data);
        result->size = strlen(result->data) + 1;

        send_message_to_coordinator(*result);
    }
    free(result->data);
    free(result);
    
    return;
}