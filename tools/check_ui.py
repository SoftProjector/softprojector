#!/usr/bin/env python3
"""Cross-check C++ sources against Qt Designer .ui files.

1. Every `ui->name` used in a .cpp must be an object declared in the .ui that
   the file includes (widget, layout, action, spacer, button group ...).
   A miss is a guaranteed compile error in the generated Ui_ class.
2. Every auto-connected slot `on_<object>_<signal>(...)` declared in a header
   must reference an object that exists in that class's .ui, otherwise
   QMetaObject::connectSlotsByName prints a runtime warning and the handler
   silently never fires.
"""
import re
import sys
import xml.etree.ElementTree as ET
from pathlib import Path

SRC = Path(sys.argv[1])
errors = 0
warnings = 0

UI_METHODS = {"setupUi", "retranslateUi"}


def ui_objects(ui_path: Path):
    tree = ET.parse(ui_path)
    root = tree.getroot()
    names = {}
    for el in root.iter():
        if el.tag in ("widget", "layout", "action", "spacer", "actiongroup", "buttongroup"):
            n = el.get("name")
            if n:
                names[n] = el.get("class", el.tag)
    # <buttongroups><buttongroup name=".."/>
    return names


def strip_comments(code: str) -> str:
    code = re.sub(r"/\*.*?\*/", lambda m: "\n" * m.group(0).count("\n"), code, flags=re.S)
    code = re.sub(r"//[^\n]*", "", code)
    return code


for cpp in sorted((SRC / "sources").glob("*.cpp")):
    raw = cpp.read_text(encoding="utf-8", errors="replace")
    code = strip_comments(raw)
    incs = re.findall(r'#include\s+"ui_(\w+)\.h"', code)
    if not incs:
        continue
    for inc in incs:
        ui = SRC / "ui" / f"{inc}.ui"
        if not ui.exists():
            print(f"ERROR {cpp.name}: includes ui_{inc}.h but ui/{inc}.ui is missing")
            errors += 1
            continue
        objs = ui_objects(ui)
        used = set(re.findall(r"\bui->(\w+)", code))
        for u in sorted(used - UI_METHODS):
            if u not in objs:
                lines = [i + 1 for i, l in enumerate(code.splitlines()) if re.search(rf"\bui->{u}\b", l)]
                print(f"ERROR {cpp.name}: ui->{u} not found in ui/{inc}.ui (lines {lines[:6]})")
                errors += 1

        # auto-connect slots declared in the matching header
        hpp = SRC / "headers" / (cpp.stem + ".hpp")
        if hpp.exists():
            hcode = strip_comments(hpp.read_text(encoding="utf-8", errors="replace"))
            for m in re.finditer(r"\bvoid\s+on_(\w+?)_(\w+)\s*\(", hcode):
                obj, sig = m.group(1), m.group(2)
                # object names can contain underscores: try progressively
                full = "on_" + obj + "_" + sig
                parts = full[3:].split("_")
                found = False
                for i in range(1, len(parts)):
                    cand_obj = "_".join(parts[:i])
                    if cand_obj in objs:
                        found = True
                        break
                if not found:
                    print(f"WARN  {hpp.name}: auto-connect slot {full}() has no matching object in ui/{inc}.ui")
                    warnings += 1

print(f"\nui check: {errors} error(s), {warnings} warning(s)")
sys.exit(1 if errors else 0)
