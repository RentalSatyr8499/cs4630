%{
#include <stdio.h>
#include <stdint.h>
char* name = "";
int location = 0x00;
%}

%%

\x68[\x00-\xFF]{4}\xc3  {
    name = "Tricky jump";
    location = ftell(yyin) - yyleng;

}

\x48\x8D\x35[\x00-\xFF]{4}\xBC[\x00-\xFF]{4}\x31\x26[x\31x\66[\x00-\xFF]]{1,10}\x48\x83\xC6[\x00-\xFF]\x83\xEC[\x00-\xFF]\x75  {
    name = "Encrypted virus";
    location = ftell(yyin) - yyleng;
}

%%

int main(void) {
    yylex();
    printf("WARNING: %s: byte number: 0x%h", name, location);

    return 0;
}