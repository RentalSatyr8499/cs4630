	int found_suspect = 0;
	void add_byte(unsigned char x);

ANY (?:.|\n)
ADDRSTART (?:\x40)
SYSCALL \x0f\x05
INT80 \xCD\x80

%%

Con                 {
                        printf("**** Found Con --- possible part of Congratulations\n");
                        found_suspect = 1;
                    }

{ADDRSTART}         {
                        printf("*** Found 0x%02x -- possible part of memory address\n",
                               yytext[0]);
                        found_suspect = 1;
                    }

{SYSCALL}           {
                        printf("*** Found %02x %02x -- possible syscall instruction\n",
                               yytext[0], yytext[1]);
                        found_suspect = 1;
                    }

{INT80}             {
                        printf("*** Found %02x %02x -- possible int $0x80 instruction\n",
                               yytext[0], yytext[1]);
                        found_suspect = 1;
                    }

{ANY}               { add_byte(yytext[0]); }

<<EOF>>             { return 0; }

%%

#include <stdalign.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

/* Buffer where we place and execute machine code from.
   This is exactly one page (4096 byte on x86-64 Linux)
   in size and aligned to a page boundary so that we can mark it
   as executable without affecting the surrounding memory. */
alignas(4096) unsigned char buffer[4096];

int buffer_index = 0; 
void add_byte(unsigned char c) {
    buffer[buffer_index] = c;
    buffer_index += 1;
    if (buffer_index >= sizeof(buffer)) {
        fprintf(stderr, "*** Input too long, aborting\n");
        abort();
    }
}

int main() {
    char name[128];
    printf("Enter your name or computing ID: ");
    fflush(stdout);
    if (!fgets(name, sizeof name, stdin)) {
        fprintf(stderr, "*** Error reading name, aborting\n");
        abort();
    }
    if (name[strlen(name)-1] == '\n')
        name[strlen(name)-1] = '\0';
    printf("Enter machine code (as binary, up to 4096 bytes)\n"); 
    fflush(stdout);
    yylex();
    if (found_suspect) {
        printf("Refusing to run code --- suspicious pattern matched in scanner.\n");
    } else {
        /* This mprotect() call marks buffer as executable memory */
        mprotect(buffer, sizeof buffer, PROT_READ | PROT_WRITE | PROT_EXEC);
        long (*fptr)(void);
        fptr = (long (*)(void)) buffer;
        long result = fptr();
        printf("The result is %ld (0x%lx)\n", result, result);
    }
    if (0 == strcmp(name, "Draco Malfoy")) {
        printf("Congratulations, Draco Malfoy!\nYou have passed this challenge.\n");
    } else {
        printf("Sorry, %s. You have NOT passed this challenge.\n", name);
    }
    exit(EXIT_SUCCESS);
}

