#define _GNU_SOURCE
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 16

// write a string to stdout without using <stdio.h> functions
#define WRITE(x) write(1, x, strlen(x))

extern char *gets(char*);

char *name;

void print_pass_message() {
    WRITE("\nCongratulations, ");
    WRITE(name);
    WRITE("!\nYou have passed this challenge.\n");
    exit(0);
}

void print_fail_message() {
    WRITE("\nSorry, ");
    WRITE(name);
    WRITE(". You have not passed this challenge.\n");
    exit(1);
}

void start_message() {
    puts("This challenge displays a giant game of Lights Out");
    puts("(See also https://en.wikipedia.org/wiki/Lights_Out_(game) .)");
    puts("When you exit that game, it will determine if you passed the challenge.");
}

bool board_get(char *board, int y, int x) {
    int index = (y - 1) * SIZE + (x - 1);
    return (board[index / 8] >> (index % 8)) & 1;
}

void board_flip(char *board, int y, int x) {
    int index = (y - 1) * SIZE + (x - 1);
    board[index / 8] ^= (1 << (index % 8));
}

void lights_out() {
    char board[SIZE * SIZE / 8];
    while (true) {
        printf("Current board:\n");
        printf("       ");
        for (int x = 1; x <= SIZE; x += 1) {
            if (x % 10 == 0)
                printf("%d", x / 10);
            else
                printf(" ");
        }
        printf("\n");
        printf("Col   -");
        for (int x = 1; x <= SIZE; x += 1) {
            printf("%d", x % 10);
        }
        printf("\n");
        for (int y = 1; y <= SIZE; y += 1) {
            printf("Row %2d: ", y);
            for (int x = 1; x <= SIZE; x += 1) {
                if (board_get(board, y, x)) {
                    putchar('*');
                } else {
                    putchar(' ');
                }
            }
            putchar('\n');
        }
        printf("Enter row,col to flip (example: '3,5') or type 'q' to try passing the challenge.\n");
        char input[128];
        if (!gets(input))
            break;
        int y, x;
        if (input[0] == 'q')
            break;
        if (2 == sscanf(input, "%d,%d", &y, &x)) {
            board_flip(board, y, x);
            if (y > 1)
                board_flip(board, y - 1, x);
            if (y < SIZE)
                board_flip(board, y + 1, x);
            if (x > 1)
                board_flip(board, y, x - 1);
            if (x < SIZE)
                board_flip(board, y, x + 1);
        } else {
            printf("Invalid input [%s].\n", input);
        }
    }
}

void check_challenge() {
    if (0 == strcmp(name, "Draco Malfoy")) {
        print_pass_message(name);
    } else {
        print_fail_message(name);
    }
}

int main(void) {
    // send output to stdout immediately, even if not writing to termiinal
    setbuf(stdout, NULL);

    // read name into dyanmically allocated memory for later
    printf("Enter name: ");
    size_t namelen = 0;
    getline(&name, &namelen, stdin);
    name[strlen(name) - 1] = '\0';

    start_message();
    lights_out();
    check_challenge();
}
