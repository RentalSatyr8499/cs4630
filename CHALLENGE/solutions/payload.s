    movq $63, %rax
    incq %rax
    subq %rax, %rsp
    movq %rsp, %rdi

    leaq 0x59(%rip), %rsi
    leaq 0x8a(%rip), %rdx
    movq $56, %rcx
loop_top:
    movzbl (%rsi), %eax
    xorb (%rdx), %al
    movb %al, (%rdi)
    incq %rsi
    incq %rdx
    incq %rdi
    decq %rcx
    jnz 25

    movq %rsp, %rdi
    movq $0x4FFC12, %rax
    movq $0x0FAE32, %rbx
    xorq %rbx, %rax
    callq *%rax

    movq $63, %rax
    incq %rax
    addq %rax, %rsp

    movq $0xFE9861, %rax
    movq $0xBEBA82, %rbx
    xorq %rbx, %rax
    jmpq *%rax

msgA:
    .byte 86, 116, 18, 179, 107, 12, 223, 227, 186, 44, 162, 6, 55, 118, 233, 193, 15, 92, 14, 32, 69, 180, 233, 181, 55, 58, 197, 63, 198, 18, 172, 24, 22, 217, 121, 110, 89, 152, 211, 153, 18, 251, 127, 9, 191, 151, 17, 155, 91, 90, 96, 6, 145, 197, 172, 38
msgB:
    .byte 21, 27, 124, 212, 25, 109, 171, 150, 214, 77, 214, 111, 88, 24, 154, 237, 47, 36, 111, 81, 114, 196, 131, 148, 61, 99, 170, 74, 230, 122, 205, 110, 115, 249, 9, 15, 42, 235, 182, 253, 50, 143, 23, 96, 204, 183, 114, 243, 58, 54, 12, 99, 255, 162, 201, 8