#!/usr/bin/python3

import asyncio
import io
import signal
import socket
import subprocess
import sys

from argparse import ArgumentParser, FileType
from asyncio import StreamReader, StreamWriter
from asyncio.subprocess import Process
from dataclasses import dataclass

@dataclass
class ForwardWriterInfo:
    writer: StreamWriter | io.RawIOBase
    need_eof: bool
    pre_forwarded: bytes | str= b''
    post_forwarded: bytes | str= b''
    show_hex: bool = False

async def _forward_loop(
        reader: StreamReader,
        writer_infos: list[ForwardWriterInfo],
    ):
    while True:
        data = await reader.read(1024)
        if len(data) == 0:
            for writer_info in writer_infos:
                if writer_info.need_eof:
                    if isinstance(writer_info.writer, StreamWriter):
                        writer_info.writer.write_eof()
                    else:
                        writer_info.writer.close()
            break
        for writer_info in writer_infos:
            if len(writer_info.pre_forwarded) > 0:
                writer_info.writer.write(writer_info.pre_forwarded)
            if writer_info.show_hex:
                writer_info.writer.write(
                    data.hex().encode('utf-8')
                )
            else:
                writer_info.writer.write(data)
            if len(writer_info.pre_forwarded) > 0:
                writer_info.writer.write(writer_info.post_forwarded)

async def _wait_for_process(
        process: Process,
        label: str
    ):
    await process.wait()
    returncode = process.returncode
    if returncode < 0:
        print(f'{label} crashed with signal {signal.strsignal(-returncode)}', file=sys.stderr)
    elif returncode > 0:
        print(f'{label} exited with exit status {returncode}', file=sys.stderr)

class AttackInstance:
    def __init__(self,
            vulnerable_command: list[str],
            attack_command: list[str],
            vulnerable_env: dict[str, str] | None = None):
        self._vulnerable_command = vulnerable_command
        self._attack_command = attack_command
        self._vulnerable_env = vulnerable_env
        self._debug = False
        self._vulnerable_output_copy = None
        self._vulnerable_input_copy = None

    def set_debug(self, debug: bool):
        self._debug = debug

    def set_vulnerable_output_copy(self, out: io.RawIOBase):
        self._vulnerable_output_copy = out

    def set_vulnerable_input_copy(self, out: io.RawIOBase):
        self._vulnerable_input_copy = out

    async def _start_programs(self):
        self._vulnerable = await asyncio.create_subprocess_exec(
            *self._vulnerable_command,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            env=self._vulnerable_env,
        )
        self._vulnerable_wait = asyncio.create_task(
            _wait_for_process(self._vulnerable, self._vulnerable_command[0])
        )
        self._attack = await asyncio.create_subprocess_exec(
            *self._attack_command,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
        )
        self._attack_wait = asyncio.create_task(
            _wait_for_process(self._attack, self._attack_command[0])
        )

    async def _connect_programs(self):
        vulnerable_writers = [
            ForwardWriterInfo(self._attack.stdin, True),
            ForwardWriterInfo(sys.stdout.buffer, False,
                b'vulnerable output: [' if self._debug else b'',
                b']\n' if self._debug else b''
            ),
        ]
        if self._vulnerable_output_copy:
            vulnerable_writers += [
                ForwardWriterInfo(self._vulnerable_output_copy, True)
            ]
        self._forward_vulnerable_stdout = asyncio.create_task(
            _forward_loop(self._vulnerable.stdout, vulnerable_writers)
        )
        attack_writers = [
            ForwardWriterInfo(self._vulnerable.stdin, True)
        ]
        if self._vulnerable_input_copy:
            attack_writers += [
                ForwardWriterInfo(self._vulnerable_input_copy, True)
            ]
        if self._debug:
            attack_writers += [
                ForwardWriterInfo(sys.stderr.buffer, False,
                    b'attack output as hex: [',
                    b']\n',
                    True
                )
            ]
        self._forward_attack_stdout = asyncio.create_task(
            _forward_loop(self._attack.stdout, attack_writers)
        )

    async def _finish(self):
        await self._forward_vulnerable_stdout
        await self._forward_attack_stdout
        await self._attack_wait
        await self._vulnerable_wait

    async def run(self):
        await self._start_programs()
        await self._connect_programs()
        await self._finish()


def main():
    parser = ArgumentParser()
    parser.add_argument('--attack', nargs='+', default=[],
        help='attack program to run', required=True)
    parser.add_argument('--vulnerable-empty-env', action='store_true',
        help='run vulnerable program with empty environment')
    parser.add_argument('--vulnerable-set-env', default=[], action='append',
        help='set environment variable for vulnerable (specified as NAME=VALUE); '
             'can be specified multiple times')
    parser.add_argument('--vulnerable-input-copy',
        help="save copy of vulnerable program's input to specified file",
        type=FileType('wb'))
    parser.add_argument('--vulnerable-output-copy',
        help="save copy of vulnerable program's output to specified file",
        type=FileType('wb'))
    parser.add_argument('--debug', action='store_true', default=False,
        help='show all input/output')
    parser.add_argument('vulnerable', nargs='+', default=[],
        help='vulnerable program to run')
    args = parser.parse_args()
    vulnerable_env = None
    if args.vulnerable_empty_env:
        vulnerable_env = []
    elif len(args.vulnerable_set_env) > 0:
        vulnerable_env = list(map(lambda s: s.split('='), args.vulnerable_set_env))
    instance = AttackInstance(
        vulnerable_command=args.vulnerable,
        attack_command=args.attack,
        vulnerable_env=vulnerable_env)
    instance.set_debug(args.debug)
    if args.vulnerable_output_copy:
        instance.set_vulnerable_output_copy(args.vulnerable_output_copy)
    if args.vulnerable_input_copy:
        instance.set_vulnerable_input_copy(args.vulnerable_input_copy)
    asyncio.run(instance.run())


if __name__ == '__main__':
    main()
