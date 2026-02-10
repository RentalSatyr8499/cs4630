%{
#include <stdio.h>
#include <stdint.h>
char* name = "";
int location = 0x00;
int virus_found = 0;
%}

%%

\x0f\x84[\x00-\xff]{4}  {
    location += 6;
}

\x68[\x00-\xFF]{4}\xc3    {
    name = "Tricky jump";
    virus_found = 1;
    yyterminate();
}

\x48\x8D\x35[\x00-\xFF]{4}\xBC[\x00-\xFF]{4}\x31\x26(\x31\x66[\x00-\xFF]){0,10}\x48\x83\xC6[\x00-\xFF]\x83\xEC[\x00-\xFF]\x75     {
    name = "Encrypted virus";
    virus_found = 1;
    yyterminate();
}

.|\n    {
    location += 1;
};


%%

int main(void) {
    yylex();
    if (virus_found == 1){
        printf("WARNING: %s at 0x%x\n", name, location);
    }
    return 0;
}