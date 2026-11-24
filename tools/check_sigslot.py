#!/usr/bin/env python3
"""Signal/slot and declaration consistency checker for a Qt C++ tree.

Checks performed (all without a compiler):
  A. String-based connect(..., SIGNAL(x(T)), ..., SLOT(y(U))) : the normalized
     signature must exist as a signal / slot on the resolved sender / receiver
     class. A mismatch compiles fine but fails at runtime with only a console
     warning ("QObject::connect: No such signal/slot").
  B. Pointer-to-member connects &Class::name : the member must exist, and must
     not be overloaded (an overloaded name cannot be deduced -> compile error
     unless wrapped in qOverload<>).
  C. Every slot/method defined in a .cpp as Class::name(...) must be declared
     in Class (else compile error); every declared slot must be defined
     somewhere (else link error from the moc-generated qt_static_metacall);
     signals must NOT be defined by hand (duplicate symbol with moc output).
"""
import re
import sys
from pathlib import Path
from collections import defaultdict

ROOT = Path(sys.argv[1])
HDR_DIRS = [ROOT / "headers"]
SRC_DIRS = [ROOT / "sources"]

# Well-known Qt signals/slots used via string connects (normalized).
QT_KNOWN = {
    "signal": {
        "clicked()", "clicked(bool)", "toggled(bool)", "triggered()", "triggered(bool)",
        "textChanged(QString)", "textChanged()", "currentIndexChanged(int)", "valueChanged(int)",
        "activated()", "timeout()", "finished()", "readyRead()", "destroyed()",
        "currentRowChanged(int)", "itemSelectionChanged()", "doubleClicked(QModelIndex)",
        "downloadProgress(qint64,qint64)", "sliderReleased()", "sliderMoved(int)",
        "stateChanged(int)", "accepted()", "rejected()", "currentChanged(int)",
        "selectionChanged(QItemSelection,QItemSelection)", "errorOccurred(QNetworkReply::NetworkError)",
        "sslErrors(QList<QSslError>)", "cursorPositionChanged()", "currentFontChanged(QFont)",
    },
    "slot": {
        "close()", "accept()", "reject()", "show()", "hide()", "update()", "deleteLater()",
        "setEnabled(bool)", "setVisible(bool)", "quit()", "clear()", "setFocus()",
    },
}


def strip_comments(code):
    code = re.sub(r"/\*.*?\*/", lambda m: "\n" * m.group(0).count("\n"), code, flags=re.S)
    return re.sub(r"//[^\n]*", "", code)


BUILTIN_TAIL = {"int", "long", "char", "short", "double", "float", "bool", "qint64", "qreal"}


def norm_param(p):
    p = p.split("=")[0].strip()
    if not p or p == "void":
        return ""
    p = re.sub(r"\s+", " ", p)
    # drop parameter name
    m = re.match(r"^(.*?(?:[\w>\]]\s+|[\*&]\s*))([A-Za-z_]\w*)$", p)
    if m and m.group(2) not in BUILTIN_TAIL and not m.group(1).strip().endswith("::"):
        head = m.group(1).strip()
        if head and not head.endswith(("const", "unsigned", "signed")):
            p = head
    p = re.sub(r"\s*([&\*<>,])\s*", r"\1", p)
    # moc normalization: const T& -> T
    m = re.match(r"^const (.+)&$", p)
    if m:
        p = m.group(1)
    return p.strip()


def split_params(s):
    out, depth, cur = [], 0, ""
    for ch in s:
        if ch in "<(":
            depth += 1
        elif ch in ">)":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur)
            cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur)
    return [norm_param(x) for x in out if norm_param(x) != ""]


def sig_of(name, params):
    return f"{name}({','.join(split_params(params))})"


class ClassInfo:
    def __init__(self, name, base):
        self.name, self.base = name, base
        self.signals, self.slots = set(), set()
        self.methods = defaultdict(list)  # name -> [(kind, signature)]
        self.members = {}  # member var name -> type
        self.access = {}   # method name -> {'public','restricted'}


classes = {}
header_text = {}

for d in HDR_DIRS:
    for h in sorted(d.glob("*.hpp")):
        code = strip_comments(h.read_text(encoding="utf-8", errors="replace"))
        header_text[h.name] = code
        for cm in re.finditer(r"\bclass\s+(\w+)\s*(?::\s*(?:public|protected|private)\s+([\w:]+))?[^;{]*\{", code):
            cname, base = cm.group(1), cm.group(2)
            # find matching brace
            i, depth = cm.end(), 1
            while depth and i < len(code):
                if code[i] == "{":
                    depth += 1
                elif code[i] == "}":
                    depth -= 1
                i += 1
            body = code[cm.end(): i - 1]
            ci = ClassInfo(cname, base)
            section = "private"
            # flatten inline bodies so they don't confuse the parser
            flat = re.sub(r"\{[^{}]*\}", ";", body)
            flat = re.sub(r"\{[^{}]*\}", ";", flat)
            for stmt in re.split(r";", flat):
                s = stmt
                # section labels may be glued to the next declaration
                while True:
                    lm = re.match(r"\s*(public|private|protected)?\s*(slots|Q_SLOTS|signals|Q_SIGNALS)?\s*:(?!:)", s)
                    if lm and (lm.group(1) or lm.group(2)):
                        section = ((lm.group(1) or "") + " " + (lm.group(2) or "")).strip()
                        s = s[lm.end():]
                    else:
                        break
                s = s.strip()
                if not s or s.startswith(("Q_OBJECT", "friend", "typedef", "using", "enum", "Q_PROPERTY", "Q_ENUM")):
                    s = re.sub(r"^Q_OBJECT", "", s).strip()
                    if not s:
                        continue
                fm = re.match(r"^(?:virtual\s+|static\s+|explicit\s+|inline\s+|Q_INVOKABLE\s+)*([\w:<>,\s\*&~]*?)\b(~?\w+)\s*\((.*)\)\s*(const)?\s*(override|=\s*0|final)?\s*$", s, flags=re.S)
                if fm:
                    name, params = fm.group(2), fm.group(3)
                    kind = "method"
                    if "signals" in section or "Q_SIGNALS" in section:
                        kind = "signal"
                    elif "slots" in section or "Q_SLOTS" in section:
                        kind = "slot"
                    sg = sig_of(name, params)
                    ci.methods[name].append((kind, sg))
                    ci.access.setdefault(name, set()).add(
                        "public" if (section.startswith("public") or kind == "signal") else "restricted")
                    if kind == "signal":
                        ci.signals.add(sg)
                    elif kind == "slot":
                        ci.slots.add(sg)
                else:
                    vm = re.match(r"^([\w:<>,\s]+?)\s*\*?\s*(\w+)\s*$", s.replace("*", " * "))
                    vm2 = re.match(r"^([\w:<>]+)\s*\*\s*(\w+)$", re.sub(r"\s+", " ", s).replace(" *", "*").replace("* ", "*").replace("*", " * ").strip())
                    mv = re.match(r"^([A-Za-z_][\w:<>]*)\s*\*?\s*([A-Za-z_]\w*)(\s*,\s*\*?\s*[A-Za-z_]\w*)*$", re.sub(r"\s+", " ", s))
                    if mv:
                        t = mv.group(1)
                        for var in re.findall(r"\*?\s*([A-Za-z_]\w*)", s[len(t):]):
                            ci.members[var] = t
            classes[cname] = ci


def all_signals(cname):
    out = set()
    while cname in classes:
        out |= classes[cname].signals
        cname = classes[cname].base
    return out


def all_slots(cname):
    out = set()
    while cname in classes:
        c = classes[cname]
        out |= c.slots
        # moc also accepts Q_INVOKABLE, but plain methods are not slots
        cname = c.base
    return out


def member_type(cname, var):
    while cname in classes:
        if var in classes[cname].members:
            return classes[cname].members[var]
        cname = classes[cname].base
    return None


# Signals declared in QML root objects (connected from C++ via rootObject())
for qml in (ROOT / "qml").glob("*.qml"):
    qcode = strip_comments(qml.read_text(encoding="utf-8", errors="replace"))
    for m in re.finditer(r"^\s*signal\s+(\w+)\s*\(([^)]*)\)", qcode, flags=re.M):
        params = ",".join(p.split()[0] for p in m.group(2).split(",") if p.strip())
        QT_KNOWN["signal"].add(f"{m.group(1)}({params})")
QT_KNOWN["signal"].add("currentRowChanged(QModelIndex,QModelIndex)")  # QItemSelectionModel

errors = warnings = 0
defs = defaultdict(set)  # class -> names defined in cpp

for d in SRC_DIRS:
    for cpp in sorted(d.glob("*.cpp")):
        code = strip_comments(cpp.read_text(encoding="utf-8", errors="replace"))
        # C: definitions
        for dm in re.finditer(r"^[\w:<>\*&\s]*?\b(\w+)::(~?\w+)\s*\(", code, flags=re.M):
            cls, meth = dm.group(1), dm.group(2)
            if cls in classes:
                defs[cls].add(meth)
                if meth != cls and meth != "~" + cls and meth not in classes[cls].methods:
                    ln = code[: dm.start()].count("\n") + 1
                    print(f"ERROR {cpp.name}:{ln}: {cls}::{meth} defined but not declared in class {cls}")
                    errors += 1
                elif meth in classes[cls].methods and all(k == "signal" for k, _ in classes[cls].methods[meth]):
                    ln = code[: dm.start()].count("\n") + 1
                    print(f"ERROR {cpp.name}:{ln}: {cls}::{meth} is a signal but is defined by hand (moc also defines it)")
                    errors += 1

        # figure out which class this file implements (for `this`)
        impl = re.findall(r"^(?:[\w:<>\*&]+\s+)*(\w+)::\1\s*\(", code, flags=re.M)
        this_cls = impl[0] if impl else cpp.stem

        # A: string connects
        for cm in re.finditer(r"connect\s*\((.*?)\)\s*;", code, flags=re.S):
            call = cm.group(1)
            if "SIGNAL(" not in call:
                continue
            ln = code[: cm.start()].count("\n") + 1
            # resolve `this` from the nearest enclosing Class::method definition
            prev = list(re.finditer(r"^(?:[\w:<>\*&]+\s+)*(\w+)::~?\w+\s*\(", code[: cm.start()], flags=re.M))
            if prev and prev[-1].group(1) in classes:
                this_cls = prev[-1].group(1)
            m = re.match(r"\s*([^,]+?)\s*,\s*SIGNAL\s*\(\s*(\w+)\s*\((.*?)\)\s*\)\s*,\s*([^,]+?)\s*,\s*(SLOT|SIGNAL)\s*\(\s*(\w+)\s*\((.*?)\)\s*\)", call, flags=re.S)
            if not m:
                m2 = re.match(r"\s*([^,]+?)\s*,\s*SIGNAL\s*\(\s*(\w+)\s*\((.*?)\)\s*\)\s*,\s*(SLOT|SIGNAL)\s*\(\s*(\w+)\s*\((.*?)\)\s*\)", call, flags=re.S)
                if m2:  # 3-arg form: receiver is this
                    snd, sname, sargs, rkind, rname, rargs = m2.groups()
                    rcv = "this"
                else:
                    print(f"WARN  {cpp.name}:{ln}: could not parse connect(): {' '.join(call.split())[:110]}")
                    warnings += 1
                    continue
            else:
                snd, sname, sargs, rcv, rkind, rname, rargs = m.groups()
            s_sig, r_sig = sig_of(sname, sargs), sig_of(rname, rargs)

            def resolve(expr):
                expr = expr.strip()
                if expr == "this":
                    return this_cls
                expr = re.sub(r"^ui->", "", expr)
                expr = re.sub(r"^&", "", expr)
                return member_type(this_cls, expr)

            scls, rcls = resolve(snd), resolve(rcv)
            where = f"{cpp.name}:{ln}"
            # sender check
            if scls in classes:
                if s_sig not in all_signals(scls):
                    print(f"ERROR {where}: SIGNAL({s_sig}) not declared as a signal on {scls}  [runtime: connect fails silently]")
                    errors += 1
            elif s_sig not in QT_KNOWN["signal"]:
                found = [c for c in classes if s_sig in classes[c].signals]
                if not found:
                    print(f"WARN  {where}: SIGNAL({s_sig}) on '{snd.strip()}' not found in project or known-Qt list")
                    warnings += 1
            # receiver check
            want = all_slots if rkind == "SLOT" else all_signals
            if rcls in classes:
                if r_sig not in want(rcls):
                    print(f"ERROR {where}: {rkind}({r_sig}) not declared on {rcls}  [runtime: connect fails silently]")
                    errors += 1
            elif r_sig not in QT_KNOWN["slot" if rkind == "SLOT" else "signal"]:
                found = [c for c in classes if r_sig in (classes[c].slots | classes[c].signals)]
                if not found:
                    print(f"WARN  {where}: {rkind}({r_sig}) on '{rcv.strip()}' not found in project or known-Qt list")
                    warnings += 1

        # B: PMF connects
        for cm in re.finditer(r"connect\s*\((.*?)\)\s*;", code, flags=re.S):
            call = cm.group(1)
            ln = code[: cm.start()].count("\n") + 1
            for pm in re.finditer(r"(?<!qOverload<)(?<!QOverload<)&(\w+)::(\w+)", call):
                cls, mem = pm.groups()
                if cls not in classes:
                    continue
                c, entries = cls, []
                while c in classes:
                    entries += classes[c].methods.get(mem, [])
                    c = classes[c].base
                if not entries:
                    print(f"ERROR {cpp.name}:{ln}: &{cls}::{mem} - no such member  [compile error]")
                    errors += 1
                elif len({s for _, s in entries}) > 1:
                    pre = call[: pm.start()]
                    if not re.search(r"(qOverload|QOverload|static_cast)\s*<[^>]*>\s*\(\s*$", pre):
                        print(f"ERROR {cpp.name}:{ln}: &{cls}::{mem} is overloaded {sorted({s for _, s in entries})} - cannot be deduced in connect()  [compile error]")
                        errors += 1
                if entries:
                    # pointer-to-member needs access; string connects never did
                    acc, c2 = set(), cls
                    while c2 in classes:
                        acc |= classes[c2].access.get(mem, set())
                        c2 = classes[c2].base
                    prevd = list(re.finditer(r"^(?:[\w:<>\*&]+\s+)*(\w+)::~?\w+\s*\(", code[: cm.start()], flags=re.M))
                    encl = prevd[-1].group(1) if prevd else None
                    if "public" not in acc and encl != cls:
                        print(f"ERROR {cpp.name}:{ln}: &{cls}::{mem} is not public but is connected from {encl}  [compile error]")
                        errors += 1

# C: declared slots with no definition
for cname, ci in classes.items():
    for name, entries in ci.methods.items():
        if any(k == "slot" for k, _ in entries) and name not in defs.get(cname, set()):
            # inline-defined in header?
            if not any(re.search(rf"\b{name}\s*\([^;]*\)\s*(const)?\s*\{{", t) for t in header_text.values()):
                print(f"ERROR {cname}::{name} declared as slot but never defined  [link error]")
                errors += 1

print(f"\nsignal/slot check: {errors} error(s), {warnings} warning(s); classes parsed: {len(classes)}")
sys.exit(1 if errors else 0)
