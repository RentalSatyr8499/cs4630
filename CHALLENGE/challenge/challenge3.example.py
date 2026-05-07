#!/usr/bin/python3
import sys
import subprocess

def convert_asm(assembly):
    with open('temp.s', 'w') as fh:
        fh.write(assembly)
    subprocess.check_call(['gcc', '-c', 'temp.s'])
    subprocess.check_call(['objcopy', '-O', 'binary', 'temp.o', 'temp.bin'])
    with open('temp.bin', 'rb') as fh:
        binary = fh.read()
    return binary

with open(sys.argv[1], 'wb') as fh:
    # response to Enter your name:
    fh.write(b'Wizard in Training\n')
    # response to machine code
    fh.write(convert_asm(r'''
        # compute 5 factorial
        movq $5, %rdi
        movq $1, %rax
    top:
        imulq %rdi, %rax
        decq %rdi
        cmp $1, %rdi
        jge top
        ret
    '''))

