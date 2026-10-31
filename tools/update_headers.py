#!/usr/bin/env python3
"""src 内のコードの先頭行を、実パスを指す URL コメントに揃える。"""
import pathlib

BASE = "https://github.com/shinchankosen/cplib/blob/main/"
ROOT = pathlib.Path(__file__).resolve().parent.parent

for path in sorted((ROOT / "src").rglob("*.hpp")):
    rel = path.relative_to(ROOT).as_posix()
    data = path.read_bytes()
    first, sep, rest = data.partition(b"\n")
    eol = b"\r\n" if first.endswith(b"\r") else b"\n"
    line = ("// " + BASE + rel).encode() + eol
    if first.rstrip(b"\r").startswith(b"// " + BASE.encode()):
        data = rest
    if not data.startswith(line):
        path.write_bytes(line + data)
