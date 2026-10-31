#!/usr/bin/env python3
"""各 .hpp をテンプレートと依存先の後ろに連結し、構文チェックする。

依存先は `// depends on src/a.hpp, src/b.hpp` の形で書く。
SKIP は単体で構文チェックできないファイル。
"""
import pathlib
import re
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
SKIP = {
    "src/graph/grid_bfs.hpp",  # 関数内に貼る断片
}


def template():
    text = (ROOT / "src/template/template.cpp").read_text(encoding="utf-8")
    return text[: text.index("void solve()")]


def deps(path, seen):
    text = path.read_text(encoding="utf-8")
    for line in re.findall(r"^// depends on (.+)$", text, re.M):
        for dep in re.split(r"\s*,\s*", line.strip()):
            target = ROOT / dep
            if target not in seen:
                deps(target, seen)
    if path not in seen:
        seen.append(path)
    return seen


failed = []
targets = sys.argv[1:] or [p.relative_to(ROOT).as_posix() for p in sorted((ROOT / "src").rglob("*.hpp"))]
for rel in targets:
    if rel in SKIP:
        continue
    source = template() + "".join(p.read_text(encoding="utf-8") + "\n" for p in deps(ROOT / rel, []))
    result = subprocess.run(
        ["g++", "-std=c++20", "-fsyntax-only", "-x", "c++", "-"],
        input=source, text=True, capture_output=True,
    )
    if result.returncode:
        failed.append(rel)
        errors = [l for l in result.stderr.splitlines() if ": error:" in l]
        print(f"NG {rel}: {errors[0].split(': error: ')[-1] if errors else result.stderr[:200]} ({len(errors)} errors)")
print(f"{len(targets) - len(failed)}/{len(targets)} ok ({len(SKIP & set(targets))} skipped)")
sys.exit(1 if failed else 0)
