virus:
    movl $1, %eax 
    movl $1, %edi
    leaq string(%rip), %rsi 
    movl $37, %edx 
    syscall
    retq
string: 
    .asciz "You have been infected with a virus!\n"