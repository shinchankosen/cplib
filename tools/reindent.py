#!/usr/bin/env python3
"""行頭の空白だけを4スペース単位に変換する。行頭以外は変更しない。

引数なしなら src 内の全 .hpp が対象。各ファイルのインデント幅は自動で判定する。
"""
import collections
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent


def detect(widths):
    deltas = collections.Counter()
    for prev, cur in zip(widths, widths[1:]):
        if cur > prev:
            deltas[cur - prev] += 1
    for width, _ in deltas.most_common():
        if width in (2, 3, 4):
            return width
    return 4


def reindent(text):
    lines = text.split("\n")
    def lead(line):
        return re.match(r"[ \t]*", line).group().expandtabs(4)
    has_tab = any(line.startswith("\t") for line in lines)
    widths = [len(lead(line)) for line in lines if line.strip() and not line.lstrip().startswith("#")]
    width = 4 if has_tab else detect(widths)
    out = []
    for line in lines:
        indent = re.match(r"[ \t]*", line).group()
        if not line.strip() or line.lstrip().startswith("#"):
            out.append(line)
            continue
        n = len(indent.expandtabs(4))
        out.append(" " * round(n * 4 / width) + line[len(indent):])
    return "\n".join(out)


paths = [pathlib.Path(a) for a in sys.argv[1:]] or sorted((ROOT / "src").rglob("*.hpp"))
for path in paths:
    text = path.read_bytes().decode()
    fixed = reindent(text)
    if fixed != text:
        path.write_bytes(fixed.encode())
