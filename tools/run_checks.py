#!/usr/bin/env python3
"""Static checks for SoftProjector that need no Qt installation.

Run from the repository root:   python tools/run_checks.py
Exit code is non-zero if any ERROR is found. Used by the CI "static-checks" job.

  1. ui->member accesses vs Qt Designer .ui files       (check_ui.py)
  2. signals/slots, pointer-to-member connects,
     declared-vs-defined methods                        (check_sigslot.py)
  3. CMakeLists.txt and softProjector.pro list the same files, and every
     listed file exists (no stale or missing entries)
  4. every file referenced by softprojector.qrc exists
  5. .ui / .qrc / .ts files are well-formed XML
  6. DisplayArea.qml has balanced braces/brackets and uses no Qt 5-only
     MediaPlayer API (seek(), volume, muted)
"""
import re
import subprocess
import sys
import xml.etree.ElementTree as ET
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "src"
TOOLS = Path(__file__).resolve().parent
failed = False


def section(title):
    print(f"\n=== {title} ===")


def fail(msg):
    global failed
    failed = True
    print("ERROR", msg)


for script in ("check_ui.py", "check_sigslot.py"):
    section(script)
    r = subprocess.run([sys.executable, str(TOOLS / script), str(SRC)], text=True, capture_output=True)
    print(r.stdout.strip())
    if r.returncode != 0:
        failed = True

section("build file lists (CMake vs qmake vs disk)")
cm = (SRC / "CMakeLists.txt").read_text()
pro = (SRC / "softProjector.pro").read_text()
cm_files = set(re.findall(r"^\s+((?:sources|headers|ui|translations)/[\w./-]+)", cm, flags=re.M))
pro_files = set(re.findall(r"((?:sources|headers|ui|translations)/[\w./-]+\.(?:cpp|hpp|ui|ts))", pro))
for f in sorted(cm_files ^ pro_files):
    fail(f"{f} is listed in {'CMakeLists.txt' if f in cm_files else 'softProjector.pro'} only")
for f in sorted(cm_files | pro_files):
    if not (SRC / f).exists():
        fail(f"{f} is listed but does not exist")
on_disk = {str(p.relative_to(SRC)).replace("\\", "/") for d in ("sources", "headers", "ui")
           for p in (SRC / d).glob("*") if p.suffix in (".cpp", ".hpp", ".ui")}
for f in sorted(on_disk - cm_files):
    fail(f"{f} exists but is not built (dead file or missing from the build lists)")
print(f"{len(cm_files)} entries compared")

section("resources")
qrc = ET.parse(SRC / "softprojector.qrc").getroot()
n = 0
for f in qrc.iter("file"):
    n += 1
    if not (SRC / f.text).exists():
        fail(f"softprojector.qrc references missing file {f.text}")
print(f"{n} resource files checked")

section("XML well-formedness")
count = 0
for pattern in ("ui/*.ui", "*.qrc", "translations/*.ts"):
    for p in SRC.glob(pattern):
        count += 1
        try:
            ET.parse(p)
        except ET.ParseError as e:
            fail(f"{p.relative_to(ROOT)}: {e}")
print(f"{count} files parsed")

section("QML")
for qml in (SRC / "qml").glob("*.qml"):
    code = re.sub(r"//[^\n]*", "", re.sub(r"/\*.*?\*/", "", qml.read_text(), flags=re.S))
    code = re.sub(r'"(?:\\.|[^"\\])*"', '""', code)
    for o, c in ("{}", "()", "[]"):
        if code.count(o) != code.count(c):
            fail(f"{qml.name}: unbalanced {o}{c} ({code.count(o)} vs {code.count(c)})")
    for bad in (r"player\.seek\(", r"player\.volume\b", r"player\.muted\b"):
        if re.search(bad, code):
            fail(f"{qml.name}: Qt 5 MediaPlayer API {bad!r} (use AudioOutput / setPosition in Qt 6)")
    if "videoOutput:" not in code or "audioOutput:" not in code:
        fail(f"{qml.name}: MediaPlayer must set videoOutput and audioOutput in Qt 6")
    print(f"{qml.name} ok" if not failed else f"{qml.name} checked")

print("\nRESULT:", "FAILED" if failed else "all static checks passed")
sys.exit(1 if failed else 0)
