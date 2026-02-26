#!/usr/bin/python3

import os
import sys
os.execve('./dumbledore.exe', ['./dumbledore.exe'] + sys.argv[2:], {})