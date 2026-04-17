#!/bin/bash
set -e

gcc -Og -g -c ipc.c
gcc -Og -g -o worker worker.c ipc.o dadasubst.o
gcc -Og -g -o dadasubst main.c ipc.o

