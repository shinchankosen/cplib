#!/usr/bin/env python3
"""README のリンク切れと、README に載っていない src のコードを検出する。"""
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
links = re.findall(r"\]\((src/[^)]+)\)", (ROOT / "README.md").read_text(encoding="utf-8"))
broken = [link for link in links if not (ROOT / link).is_file()]
code = {p.relative_to(ROOT).as_posix() for p in (ROOT / "src").rglob("*") if p.suffix in (".hpp", ".cpp")}
missing = sorted(code - set(links))

for link in broken:
    print("broken:", link)
for path in missing:
    print("not in README:", path)
sys.exit(1 if broken or missing else 0)
