#!/usr/bin/env python3
"""SoftProjector icon set generator (v3.0 design system).

Every icon is described ONCE as vector primitives on a 24-unit grid (or a
custom view box for illustrations). From that single description this script
writes:

  * src/icons/svg/<name>.svg      editable vector master
  * src/icons/<name>.png          PNG at the original pixel size (all code/.ui
                                  references stay unchanged)
  * src/icons/<name>@2x.png       high-DPI variant, picked up automatically by
                                  QIcon / QML Image on 150-300 % displays
  * src/softprojector.ico         Windows icon, each size rendered natively
  * src/translations/flag_*.png   language flags in the same style
  * ICONS.md                      design system + per-icon mapping to the
                                  equivalent iOS (SF Symbols) / Android
                                  (Material Symbols) metaphors

Rendering: Pillow only (no SVG library needed). Shapes are drawn at 8x
supersampling in premultiplied alpha and box-downsampled, which gives clean
anti-aliasing.

Run from the repository root:   python tools/icons/build_icons.py
"""
import colorsys
import math
import pathlib
from PIL import Image, ImageDraw, ImageFilter, ImageFont

try:
    import numpy as np
except ImportError:  # gradients fall back to a solid mid colour
    np = None

ROOT = pathlib.Path(__file__).resolve().parents[2]
SRC = ROOT / "src"
ICON_DIR = SRC / "icons"
SVG_DIR = ICON_DIR / "svg"
FLAG_DIR = SRC / "translations"

# --------------------------------------------------------------------------
# Colour system
# --------------------------------------------------------------------------
# Qt does not tint PNG icons per theme, so one asset has to work on the light
# theme (#FFFFFF / #F0F0F0) AND the app's dark theme (#2D2D2D window, #121212
# lists). WCAG 2.2 SC 1.4.11 asks for >= 3:1 contrast for UI graphics. Solving
# for both backgrounds gives a relative-luminance band of ~0.18 - 0.26; every
# glyph colour below is tuned to L = 0.215 (about 3.5:1 on both themes).
TARGET_L = 0.215
BACKGROUNDS = {"light #FFFFFF": (255, 255, 255), "light #F0F0F0": (240, 240, 240),
               "dark #2D2D2D": (45, 45, 45), "dark #121212": (18, 18, 18)}


def _lin(c):
    c /= 255
    return c / 12.92 if c <= 0.04045 else ((c + 0.055) / 1.055) ** 2.4


def rel_lum(rgb):
    r, g, b = (_lin(v) for v in rgb[:3])
    return 0.2126 * r + 0.7152 * g + 0.0722 * b


def contrast(a, b):
    la, lb = rel_lum(a), rel_lum(b)
    return (max(la, lb) + 0.05) / (min(la, lb) + 0.05)


def tuned(hue, sat, target=TARGET_L):
    lo, hi = 0.0, 1.0
    for _ in range(40):
        mid = (lo + hi) / 2
        r, g, b = colorsys.hls_to_rgb(hue / 360, mid, sat)
        if rel_lum((r * 255, g * 255, b * 255)) < target:
            lo = mid
        else:
            hi = mid
    r, g, b = colorsys.hls_to_rgb(hue / 360, (lo + hi) / 2, sat)
    return (round(r * 255), round(g * 255), round(b * 255), 255)


INK = tuned(215, 0.14)      # neutral glyph colour (toolbar, actions)
ACCENT = tuned(217, 0.90)   # primary / brand blue
RED = tuned(356, 0.78)      # destructive
GREEN = tuned(145, 0.62)    # go live
VIOLET = tuned(262, 0.72)   # songs
ORANGE = tuned(28, 0.88)    # media
TEAL = tuned(178, 0.70)     # announcements
WHITE = (255, 255, 255, 255)
PALETTE = {"Ink (neutral)": INK, "Accent / Bible": ACCENT, "Destructive": RED, "Go live / Pictures": GREEN,
           "Songs": VIOLET, "Media": ORANGE, "Announcements": TEAL}

BRAND_A, BRAND_B = (59, 130, 246, 255), (109, 40, 217, 255)   # app icon gradient


def rgba(c, a=None):
    c = tuple(c) + (255,) * (4 - len(c))
    return c if a is None else (c[0], c[1], c[2], int(round(a * 255)))


# --------------------------------------------------------------------------
# Vector primitives (plain dicts; coordinates in view-box units)
# --------------------------------------------------------------------------
def L(pts, c=INK, w=2.0, closed=False):
    return {"op": "line", "pts": pts, "c": c, "w": w, "closed": closed}


def C(cx, cy, r, fill=None, stroke=None, w=2.0):
    return {"op": "ellipse", "cx": cx, "cy": cy, "rx": r, "ry": r, "fill": fill, "stroke": stroke, "w": w}


def E(cx, cy, rx, ry, fill=None, stroke=None, w=2.0):
    return {"op": "ellipse", "cx": cx, "cy": cy, "rx": rx, "ry": ry, "fill": fill, "stroke": stroke, "w": w}


def RR(x, y, w, h, r, fill=None, stroke=None, sw=2.0, grad=None):
    return {"op": "rrect", "x": x, "y": y, "w": w, "h": h, "r": r, "fill": fill, "stroke": stroke,
            "sw": sw, "grad": grad}


def POLY(pts, fill, rnd=0.0):
    return {"op": "poly", "pts": pts, "fill": fill, "rnd": rnd}


def ARC(cx, cy, r, a0, a1, c=INK, w=2.0, ry=None):
    return {"op": "arc", "cx": cx, "cy": cy, "rx": r, "ry": ry if ry else r, "a0": a0, "a1": a1, "c": c, "w": w}


def TXT(x, y, text, size, c, bold=False):
    return {"op": "text", "x": x, "y": y, "text": text, "size": size, "c": c, "bold": bold}


def G(ops, opacity=1.0, clip=None, blur=0.0, dx=0.0, dy=0.0):
    return {"op": "group", "ops": ops, "opacity": opacity, "clip": clip, "blur": blur, "dx": dx, "dy": dy}


def ERASE(ops):
    return {"op": "erase", "ops": ops}


def xform(ops, s, dx=0.0, dy=0.0):
    """Scale + translate ops (used to place the app icon inside the splash/logo)."""
    out = []
    for o in ops:
        o = dict(o)
        k = o["op"]
        P = lambda p: (p[0] * s + dx, p[1] * s + dy)
        if k in ("line", "poly"):
            o["pts"] = [P(p) for p in o["pts"]]
            if k == "line":
                o["w"] *= s
            else:
                o["rnd"] *= s
        elif k in ("ellipse", "arc"):
            o["cx"], o["cy"] = P((o["cx"], o["cy"]))
            o["rx"] *= s
            o["ry"] *= s
            o["w"] *= s
        elif k == "rrect":
            o["x"], o["y"] = P((o["x"], o["y"]))
            for key in ("w", "h", "r", "sw"):
                o[key] *= s
            if o["grad"]:
                x1, y1, x2, y2, c1, c2 = o["grad"]
                o["grad"] = (*P((x1, y1)), *P((x2, y2)), c1, c2)
        elif k == "text":
            o["x"], o["y"] = P((o["x"], o["y"]))
            o["size"] *= s
        elif k in ("group", "erase"):
            o["ops"] = xform(o["ops"], s, dx, dy)
            if k == "group":
                o["blur"] *= s
                o["dx"] *= s
                o["dy"] *= s
                if o["clip"]:
                    x, y, w, h, r = o["clip"]
                    o["clip"] = (x * s + dx, y * s + dy, w * s, h * s, r * s)
        out.append(o)
    return out


# --------------------------------------------------------------------------
# Fonts (for brand artwork only)
# --------------------------------------------------------------------------
FONT_CANDIDATES = {
    True: ["Inter-Bold.ttf", "Roboto-Bold.ttf", "NotoSans-Bold.ttf", "LiberationSans-Bold.ttf", "DejaVuSans-Bold.ttf"],
    False: ["Inter-Regular.ttf", "Roboto-Regular.ttf", "NotoSans-Regular.ttf", "LiberationSans-Regular.ttf",
            "DejaVuSans.ttf"],
}
SVG_FONT_STACK = "'Segoe UI Variable Display','Segoe UI',Inter,Roboto,'Helvetica Neue',Arial,sans-serif"
_font_cache = {}


def font_path(bold):
    if bold in _font_cache:
        return _font_cache[bold]
    found = None
    for d in ("/usr/share/fonts", "/usr/local/share/fonts", "C:/Windows/Fonts", "/Library/Fonts",
              "/System/Library/Fonts"):
        base = pathlib.Path(d)
        if not base.exists():
            continue
        for name in FONT_CANDIDATES[bold]:
            hits = list(base.rglob(name))
            if hits:
                found = str(hits[0])
                break
        if found:
            break
    _font_cache[bold] = found
    return found


def load_font(size_px, bold):
    p = font_path(bold)
    return ImageFont.truetype(p, max(1, int(round(size_px)))) if p else ImageFont.load_default()


# --------------------------------------------------------------------------
# Raster renderer
# --------------------------------------------------------------------------
class Raster:
    def __init__(self, w, h, view, ss):
        vw, vh = view
        self.W, self.H = int(w * ss), int(h * ss)
        self.S = min(w / vw, h / vh) * ss
        self.ox = (self.W - vw * self.S) / 2
        self.oy = (self.H - vh * self.S) / 2

    def P(self, x, y):
        return (self.ox + x * self.S, self.oy + y * self.S)

    def blank(self, mode="RGBA"):
        return Image.new(mode, (self.W, self.H), 0)

    # -- shapes onto a draw object with a solid fill value --------------------
    def paint(self, d, o, val):
        S, P = self.S, self.P
        k = o["op"]
        if k == "line":
            pts = [P(*p) for p in o["pts"]]
            if o["closed"]:
                pts.append(pts[0])
            wpx = o["w"] * S
            d.line(pts, fill=val, width=max(1, int(round(wpx))), joint="curve")
            r = wpx / 2
            for x, y in pts:
                d.ellipse([x - r, y - r, x + r, y + r], fill=val)
        elif k == "ellipse":
            cx, cy = P(o["cx"], o["cy"])
            rx, ry = o["rx"] * S, o["ry"] * S
            if o["fill"] is not None:
                d.ellipse([cx - rx, cy - ry, cx + rx, cy + ry], fill=val)
            if o["stroke"] is not None:
                h = o["w"] * S / 2
                d.ellipse([cx - rx - h, cy - ry - h, cx + rx + h, cy + ry + h], outline=val,
                          width=max(1, int(round(o["w"] * S))))
        elif k == "rrect":
            x0, y0 = P(o["x"], o["y"])
            x1, y1 = P(o["x"] + o["w"], o["y"] + o["h"])
            r = o["r"] * S
            if o["fill"] is not None or o["grad"] is not None:
                d.rounded_rectangle([x0, y0, x1, y1], radius=r, fill=val)
            if o["stroke"] is not None:
                h = o["sw"] * S / 2
                d.rounded_rectangle([x0 - h, y0 - h, x1 + h, y1 + h], radius=r + h, outline=val,
                                    width=max(1, int(round(o["sw"] * S))))
        elif k == "poly":
            pts = [P(*p) for p in o["pts"]]
            d.polygon(pts, fill=val)
            if o["rnd"]:
                wpx = 2 * o["rnd"] * S
                d.line(pts + [pts[0]], fill=val, width=max(1, int(round(wpx))), joint="curve")
                for x, y in pts:
                    d.ellipse([x - wpx / 2, y - wpx / 2, x + wpx / 2, y + wpx / 2], fill=val)
        elif k == "arc":
            cx, cy = P(o["cx"], o["cy"])
            rx, ry = o["rx"] * S, o["ry"] * S
            h = o["w"] * S / 2
            d.arc([cx - rx - h, cy - ry - h, cx + rx + h, cy + ry + h], o["a0"], o["a1"], fill=val,
                  width=max(1, int(round(o["w"] * S))))
            for a in (o["a0"], o["a1"]):
                x, y = cx + rx * math.cos(math.radians(a)), cy + ry * math.sin(math.radians(a))
                d.ellipse([x - h, y - h, x + h, y + h], fill=val)
        elif k == "text":
            font = load_font(o["size"] * S, o["bold"])
            d.text(P(o["x"], o["y"]), o["text"], fill=val, font=font, anchor="ls")

    def shape_mask(self, o):
        m = self.blank("L")
        self.paint(ImageDraw.Draw(m), o, 255)
        return m

    def gradient(self, grad):
        x1, y1, x2, y2, c1, c2 = grad
        c1, c2 = rgba(c1), rgba(c2)
        if np is None:
            return Image.new("RGBA", (self.W, self.H), tuple((a + b) // 2 for a, b in zip(c1, c2)))
        (px1, py1), (px2, py2) = self.P(x1, y1), self.P(x2, y2)
        dx, dy = px2 - px1, py2 - py1
        yy, xx = np.mgrid[0:self.H, 0:self.W].astype(np.float32)
        t = np.clip(((xx - px1) * dx + (yy - py1) * dy) / max(dx * dx + dy * dy, 1e-6), 0, 1)[..., None]
        a, b = np.array(c1, np.float32), np.array(c2, np.float32)
        return Image.fromarray((a + (b - a) * t).round().astype(np.uint8), "RGBA")

    # -- ops onto an RGBA canvas ---------------------------------------------
    def render(self, ops, img):
        for o in ops:
            k = o["op"]
            if k == "erase":
                m = self.blank("L")
                d = ImageDraw.Draw(m)
                for so in o["ops"]:
                    self.paint(d, so, 255)
                alpha = img.getchannel("A")
                keep = Image.eval(m, lambda v: 255 - v)
                img.putalpha(Image.composite(alpha, Image.new("L", alpha.size, 0), keep))
            elif k == "group":
                layer = self.blank()
                self.render(o["ops"], layer)
                if o["blur"]:
                    layer = layer.filter(ImageFilter.GaussianBlur(o["blur"] * self.S))
                if o["dx"] or o["dy"]:
                    moved = self.blank()
                    moved.paste(layer, (int(round(o["dx"] * self.S)), int(round(o["dy"] * self.S))))
                    layer = moved
                a = layer.getchannel("A")
                if o["opacity"] < 1:
                    a = a.point(lambda v, f=o["opacity"]: int(v * f))
                if o["clip"]:
                    x, y, w, h, r = o["clip"]
                    cm = self.shape_mask(RR(x, y, w, h, r, fill=1))
                    a = Image.composite(a, Image.new("L", a.size, 0), cm)
                layer.putalpha(a)
                img.alpha_composite(layer)
            else:
                layer = self.blank()
                if k == "rrect" and o["grad"] is not None:
                    fill = self.gradient(o["grad"])
                    m = self.shape_mask(dict(o, stroke=None))
                    fa = Image.composite(fill.getchannel("A"), Image.new("L", m.size, 0), m)
                    fill.putalpha(fa)
                    layer = fill
                    if o["stroke"] is not None:
                        self.paint(ImageDraw.Draw(layer), dict(o, fill=None, grad=None), rgba(o["stroke"]))
                else:
                    col = rgba(o.get("c") or o.get("fill") or o.get("stroke"))
                    if k in ("ellipse", "rrect") and o["fill"] is not None and o["stroke"] is not None:
                        self.paint(ImageDraw.Draw(layer), dict(o, stroke=None), rgba(o["fill"]))
                        self.paint(ImageDraw.Draw(layer), dict(o, fill=None, grad=None), rgba(o["stroke"]))
                    else:
                        self.paint(ImageDraw.Draw(layer), o, col)
                img.alpha_composite(layer)
        return img


def rasterize(ops, w, h, view, ss=8):
    ss = 4 if w * h > 120_000 else ss          # keep big artwork memory-friendly
    r = Raster(w, h, view, ss)
    img = r.render(ops, r.blank())
    # premultiplied box downsample: no dark fringes on transparent edges
    return img.convert("RGBa").reduce(ss).convert("RGBA")


# --------------------------------------------------------------------------
# SVG writer
# --------------------------------------------------------------------------
class SvgWriter:
    def __init__(self):
        self.defs = []
        self.n = 0

    def uid(self, p):
        self.n += 1
        return f"{p}{self.n}"

    @staticmethod
    def col(c):
        c = rgba(c)
        s = f'#{c[0]:02x}{c[1]:02x}{c[2]:02x}'
        return s, (f' {{kind}}-opacity="{c[3] / 255:.3f}"' if c[3] < 255 else "")

    def paint_attrs(self, fill=None, stroke=None, w=None):
        a = []
        if fill is not None:
            h, op = self.col(fill)
            a.append(f'fill="{h}"' + op.format(kind="fill"))
        else:
            a.append('fill="none"')
        if stroke is not None:
            h, op = self.col(stroke)
            a.append(f'stroke="{h}" stroke-width="{w:g}" stroke-linecap="round" stroke-linejoin="round"'
                     + op.format(kind="stroke"))
        return " ".join(a)

    def el(self, o, mono=None):
        """mono: when set (mask content), paint every shape with this colour."""
        k = o["op"]
        f = lambda n: f"{n:.3f}".rstrip("0").rstrip(".")
        if k == "line":
            pts = " ".join(f"{f(x)},{f(y)}" for x, y in o["pts"])
            tag = "polygon" if o["closed"] else "polyline"
            return f'<{tag} points="{pts}" {self.paint_attrs(None, mono or o["c"], o["w"])}/>'
        if k == "ellipse":
            out = []
            if o["fill"] is not None:
                out.append(f'<ellipse cx="{f(o["cx"])}" cy="{f(o["cy"])}" rx="{f(o["rx"])}" ry="{f(o["ry"])}" '
                           f'{self.paint_attrs(mono or o["fill"])}/>')
            if o["stroke"] is not None:
                out.append(f'<ellipse cx="{f(o["cx"])}" cy="{f(o["cy"])}" rx="{f(o["rx"])}" ry="{f(o["ry"])}" '
                           f'{self.paint_attrs(None, mono or o["stroke"], o["w"])}/>')
            return "".join(out)
        if k == "rrect":
            geo = f'x="{f(o["x"])}" y="{f(o["y"])}" width="{f(o["w"])}" height="{f(o["h"])}" rx="{f(o["r"])}"'
            out = []
            if o["grad"] is not None and not mono:
                gid = self.uid("g")
                x1, y1, x2, y2, c1, c2 = o["grad"]
                (h1, _), (h2, _) = self.col(c1), self.col(c2)
                self.defs.append(
                    f'<linearGradient id="{gid}" gradientUnits="userSpaceOnUse" x1="{f(x1)}" y1="{f(y1)}" '
                    f'x2="{f(x2)}" y2="{f(y2)}"><stop offset="0" stop-color="{h1}" stop-opacity="{rgba(c1)[3] / 255:.3f}"/>'
                    f'<stop offset="1" stop-color="{h2}" stop-opacity="{rgba(c2)[3] / 255:.3f}"/></linearGradient>')
                out.append(f'<rect {geo} fill="url(#{gid})"/>')
            elif o["fill"] is not None or o["grad"] is not None:
                out.append(f'<rect {geo} {self.paint_attrs(mono or o["fill"] or (0, 0, 0))}/>')
            if o["stroke"] is not None:
                out.append(f'<rect {geo} {self.paint_attrs(None, mono or o["stroke"], o["sw"])}/>')
            return "".join(out)
        if k == "poly":
            pts = " ".join(f"{f(x)},{f(y)}" for x, y in o["pts"])
            c = mono or o["fill"]
            extra = self.paint_attrs(c, c, 2 * o["rnd"]) if o["rnd"] else self.paint_attrs(c)
            return f'<polygon points="{pts}" {extra}/>'
        if k == "arc":
            a0, a1 = math.radians(o["a0"]), math.radians(o["a1"])
            x0, y0 = o["cx"] + o["rx"] * math.cos(a0), o["cy"] + o["ry"] * math.sin(a0)
            x1, y1 = o["cx"] + o["rx"] * math.cos(a1), o["cy"] + o["ry"] * math.sin(a1)
            large = 1 if (o["a1"] - o["a0"]) % 360 > 180 else 0
            d = f'M{f(x0)} {f(y0)} A{f(o["rx"])} {f(o["ry"])} 0 {large} 1 {f(x1)} {f(y1)}'
            return f'<path d="{d}" {self.paint_attrs(None, mono or o["c"], o["w"])}/>'
        if k == "text":
            h, op = self.col(mono or o["c"])
            weight = "700" if o["bold"] else "400"
            return (f'<text x="{f(o["x"])}" y="{f(o["y"])}" font-family="{SVG_FONT_STACK}" font-size="{f(o["size"])}" '
                    f'font-weight="{weight}" fill="{h}"{op.format(kind="fill")}>{o["text"]}</text>')
        if k == "group":
            attrs = []
            if o["opacity"] < 1:
                attrs.append(f'opacity="{o["opacity"]:.3f}"')
            if o["dx"] or o["dy"]:
                attrs.append(f'transform="translate({f(o["dx"])} {f(o["dy"])})"')
            if o["blur"]:
                fid = self.uid("f")
                self.defs.append(f'<filter id="{fid}" x="-50%" y="-50%" width="200%" height="200%">'
                                 f'<feGaussianBlur stdDeviation="{f(o["blur"])}"/></filter>')
                attrs.append(f'filter="url(#{fid})"')
            inner = "".join(self.el(so, mono) for so in o["ops"])
            g = f'<g {" ".join(attrs)}>{inner}</g>'
            if o["clip"]:
                cid = self.uid("c")
                x, y, w, h, r = o["clip"]
                self.defs.append(f'<clipPath id="{cid}"><rect x="{f(x)}" y="{f(y)}" width="{f(w)}" '
                                 f'height="{f(h)}" rx="{f(r)}"/></clipPath>')
                g = f'<g clip-path="url(#{cid})">{g}</g>'
            return g
        return ""

    def document(self, ops, w, h, view):
        body = []
        for o in ops:
            if o["op"] == "erase":
                mid = self.uid("m")
                cut = "".join(self.el(so, mono=(0, 0, 0)) for so in o["ops"])
                self.defs.append(f'<mask id="{mid}" maskUnits="userSpaceOnUse" x="-1000" y="-1000" '
                                 f'width="3000" height="3000"><rect x="-1000" y="-1000" width="3000" '
                                 f'height="3000" fill="#ffffff"/>{cut}</mask>')
                body = [f'<g mask="url(#{mid})">{"".join(body)}</g>']
            else:
                body.append(self.el(o))
        defs = f"<defs>{''.join(self.defs)}</defs>" if self.defs else ""
        return (f'<svg xmlns="http://www.w3.org/2000/svg" width="{w}" height="{h}" '
                f'viewBox="0 0 {view[0]:g} {view[1]:g}">{defs}{"".join(body)}</svg>\n')


# --------------------------------------------------------------------------
# Glyph library (24-unit grid, 2-unit safe margin, 2-unit strokes)
# --------------------------------------------------------------------------
def g_plus(c=INK):
    return [L([(5, 12), (19, 12)], c), L([(12, 5), (12, 19)], c)]


def g_xmark(c=INK):
    return [L([(6.5, 6.5), (17.5, 17.5)], c), L([(17.5, 6.5), (6.5, 17.5)], c)]


def g_book(c=INK):
    return [L([(12, 7), (4.5, 5), (4.5, 18), (12, 20)], c), L([(12, 7), (19.5, 5), (19.5, 18), (12, 20)], c),
            L([(12, 7), (12, 20)], c)]


def g_note(c=INK):
    return [C(8.6, 17, 3.1, fill=c), L([(11.7, 17), (11.7, 4.2)], c, 2.2),
            POLY([(11.7, 3.4), (17.6, 5.3), (17.6, 8.6), (11.7, 6.8)], c, rnd=0.7)]


def g_photo(c=INK):
    return [RR(3, 4.5, 18, 15, 2.5, stroke=c), C(8.5, 9.4, 1.7, fill=c),
            L([(3.8, 17.6), (9.3, 12.4), (13, 15.8), (15.6, 13.3), (20.2, 17.4)], c)]


def g_video(c=INK):
    return [RR(2.5, 6, 13, 12, 2.5, stroke=c), L([(15.5, 10.4), (21, 7.6), (21, 16.4), (15.5, 13.6)], c, closed=True)]


def g_announce(c=INK):
    return [L([(7.5, 9), (15, 5.2), (15, 18.8), (7.5, 15)], c, closed=True), RR(3.5, 9, 4, 6, 1.2, stroke=c),
            L([(6.2, 15.2), (7.6, 19.4)], c), L([(18.4, 12), (21, 12)], c),
            L([(18.2, 7.9), (20.4, 6.4)], c), L([(18.2, 16.1), (20.4, 17.6)], c)]


def g_slideshow(c=INK):
    return [RR(3, 4, 18, 16, 2.5, stroke=c), POLY([(10.4, 9.2), (14.8, 12), (10.4, 14.8)], c, rnd=0.9)]


def g_monitor(c=INK, filled=False):
    body = RR(2, 3.5, 20, 13, 2.8, fill=c) if filled else RR(3, 4.5, 18, 11, 2, stroke=c)
    return [body, L([(12, 16.5), (12, 19.5)], c), L([(8, 20), (16, 20)], c)]


def g_present(c=INK, inner="up"):
    ops = [RR(2.5, 4.5, 19, 15, 2.5, stroke=c)]
    if inner == "up":
        ops += [L([(12, 16), (12, 8.6)], c), L([(8.6, 12), (12, 8.6), (15.4, 12)], c)]
    else:
        ops += [L([(9.2, 9.2), (14.8, 14.8)], c), L([(14.8, 9.2), (9.2, 14.8)], c)]
    return ops


def g_pencil(c=INK):
    return [L([(15.6, 5.2), (18.8, 8.4), (8.6, 18.6), (4.6, 19.4), (5.4, 15.4)], c, closed=True),
            L([(13.4, 7.4), (16.6, 10.6)], c)]


def g_search(c=INK):
    return [C(10.5, 10.5, 6.3, stroke=c), L([(15.2, 15.2), (20.3, 20.3)], c)]


def gear_points(cx=12, cy=12, r_out=9.0, r_in=6.9, teeth=8, top=0.40, base=0.62):
    pts = []
    for i in range(teeth):
        a = 2 * math.pi * i / teeth
        ht, hb = math.pi / teeth * top, math.pi / teeth * base
        for ang, r in ((a - hb, r_in), (a - ht, r_out), (a + ht, r_out), (a + hb, r_in)):
            pts.append((cx + r * math.cos(ang), cy + r * math.sin(ang)))
    return pts


def g_gear(c=INK):
    return [L(gear_points(), c, 1.9, closed=True), C(12, 12, 2.9, stroke=c, w=1.9)]


def g_help(c=INK):
    return [C(12, 12, 9.4, stroke=c), ARC(12, 9.6, 2.9, 180, 405, c),
            L([(14.05, 11.65), (12, 13.2), (12, 14.2)], c), C(12, 17.3, 1.25, fill=c)]


def g_printer(c=INK):
    return [RR(3, 8.5, 18, 8.5, 2, stroke=c), L([(7, 8.5), (7, 4), (17, 4), (17, 8.5)], c),
            ERASE([RR(5.4, 12.4, 13.2, 10, 1, fill=1)]), RR(7, 14, 10, 6.5, 1, stroke=c), C(17.4, 11.3, 1, fill=c)]


def g_database(c=INK):
    return [E(12, 6, 7.5, 2.8, stroke=c), L([(4.5, 6), (4.5, 18)], c), L([(19.5, 6), (19.5, 18)], c),
            ARC(12, 18, 7.5, 0, 180, c, ry=2.8), ARC(12, 12, 7.5, 0, 180, c, ry=2.8)]


def g_speaker(c=INK, muted=False):
    ops = [L([(3.5, 9.2), (7.5, 9.2), (12.5, 5), (12.5, 19), (7.5, 14.8), (3.5, 14.8)], c, closed=True)]
    if muted:
        ops += [L([(16, 9.5), (21, 14.5)], c), L([(21, 9.5), (16, 14.5)], c)]
    else:
        ops += [ARC(13, 12, 3.8, -45, 45, c), ARC(13, 12, 7.3, -52, 52, c)]
    return ops


def g_play(c=INK):
    return [POLY([(8.8, 6.4), (17.9, 12), (8.8, 17.6)], c, rnd=1.3)]


def g_pause(c=INK):
    return [RR(6, 5, 4.2, 14, 1.6, fill=c), RR(13.8, 5, 4.2, 14, 1.6, fill=c)]


def g_stop(c=INK):
    return [RR(5.5, 5.5, 13, 13, 2.8, fill=c)]


def g_arrow(c=INK, up=True):
    if up:
        return [L([(12, 19.5), (12, 5)], c), L([(6.5, 10.5), (12, 5), (17.5, 10.5)], c)]
    return [L([(12, 4.5), (12, 19)], c), L([(6.5, 13.5), (12, 19), (17.5, 13.5)], c)]


def g_to_edge(c=INK, top=True):
    if top:
        return [L([(5, 4), (19, 4)], c), L([(12, 20), (12, 8.6)], c), L([(7.6, 13), (12, 8.6), (16.4, 13)], c)]
    return [L([(5, 20), (19, 20)], c), L([(12, 4), (12, 15.4)], c), L([(7.6, 11), (12, 15.4), (16.4, 11)], c)]


def g_list(c=INK):
    return [L([(3, 6), (15, 6)], c), L([(3, 11), (15, 11)], c), L([(3, 16), (10, 16)], c)]


def g_doc_plus(c=INK):
    return [L([(13.5, 3), (6, 3), (6, 21), (18, 21), (18, 7.5)], c, closed=True),
            L([(13.5, 3), (13.5, 7.5), (18, 7.5)], c), L([(12, 11), (12, 17)], c), L([(9, 14), (15, 14)], c)]


def g_folder_open(c=INK):
    return [L([(3, 18.5), (3, 5.5), (9, 5.5), (11, 7.5), (18.5, 7.5), (18.5, 10.5)], c),
            L([(3, 18.5), (6, 11), (21.5, 11), (18.5, 18.5)], c, closed=True)]


def g_tray_down(c=INK):
    return [L([(4, 14), (4, 19.5), (20, 19.5), (20, 14)], c), L([(12, 3.5), (12, 14.5)], c),
            L([(8, 10.5), (12, 14.5), (16, 10.5)], c)]


def g_trash(c=INK):
    return [L([(4, 6.5), (20, 6.5)], c), L([(9.5, 6.5), (9.5, 4), (14.5, 4), (14.5, 6.5)], c),
            L([(6, 6.5), (7.2, 20), (16.8, 20), (18, 6.5)], c), L([(10, 10.2), (10, 16.2)], c),
            L([(14, 10.2), (14, 16.2)], c)]


def g_clock(c=INK):
    return [C(11, 11, 8, stroke=c), L([(11, 6.8), (11, 11), (14, 13)], c)]


def g_layers_check(c=INK):
    return [RR(8, 3, 13, 13, 2.5, stroke=c), ERASE([RR(0.5, 7.5, 17, 17, 4, fill=1)]),
            RR(3, 9, 12, 12, 2.5, stroke=c), L([(6.2, 15.2), (8.4, 17.4), (12, 12.8)], c)]


def g_format_clear(c=INK):
    return [L([(6, 5), (18, 5)], c), L([(12, 5), (12, 19)], c), ERASE([L([(4, 4), (20, 20)], w=5.5)]),
            L([(4.5, 4.5), (19.5, 19.5)], c)]


def g_logout(c=INK):
    return [L([(10, 4), (5, 4), (5, 20), (10, 20)], c), L([(10, 12), (20, 12)], c),
            L([(16, 8), (20, 12), (16, 16)], c)]


def g_delete_sweep(c=RED):
    return [L([(2.5, 9), (7, 9)], c), L([(2.5, 13), (6, 13)], c), L([(2.5, 17), (5, 17)], c),
            L([(9.5, 7), (21, 7)], c), L([(13.2, 7), (13.2, 4.8), (17.3, 4.8), (17.3, 7)], c),
            L([(11, 7), (12, 19.5), (18.5, 19.5), (19.5, 7)], c)]


def badge(kind, color):
    cx, cy, r = 18.5, 18.5, 5.5
    ops = [ERASE([C(cx, cy, r + 1.6, fill=1)]), C(cx, cy, r, fill=color)]
    w, W = 1.6, WHITE
    if kind == "plus":
        ops += [L([(cx - 2.6, cy), (cx + 2.6, cy)], W, w), L([(cx, cy - 2.6), (cx, cy + 2.6)], W, w)]
    elif kind == "minus":
        ops += [L([(cx - 2.6, cy), (cx + 2.6, cy)], W, w)]
    elif kind == "x":
        ops += [L([(cx - 2, cy - 2), (cx + 2, cy + 2)], W, w), L([(cx + 2, cy - 2), (cx - 2, cy + 2)], W, w)]
    elif kind == "pencil":
        ops += [L([(cx - 2.4, cy + 2.4), (cx + 1.8, cy - 1.8)], W, 1.9), L([(cx + 1.0, cy - 2.8), (cx + 2.8, cy - 1.0)], W, 1.3)]
    elif kind == "copy":
        ops += [RR(cx - 0.9, cy - 3.0, 3.8, 3.8, 1, stroke=W, sw=1.2), RR(cx - 2.9, cy - 0.9, 3.8, 3.8, 1, fill=W)]
    elif kind == "chart":
        ops += [L([(cx - 2.3, cy + 2.4), (cx - 2.3, cy + 0.6)], W, 1.5), L([(cx, cy + 2.4), (cx, cy - 2.4)], W, 1.5),
                L([(cx + 2.3, cy + 2.4), (cx + 2.3, cy - 0.8)], W, 1.5)]
    return ops


def with_badge(base, kind, color):
    return base + badge(kind, color)


# --------------------------------------------------------------------------
# Small illustrations & special artwork
# --------------------------------------------------------------------------
def g_align(kind, c=INK):
    rows = (5, 9.5, 14, 18.5)
    lengths = (16, 10, 16, 8)
    ops = []
    for y, n in zip(rows, lengths):
        if kind == "left":
            ops.append(L([(4, y), (4 + n, y)], c))
        elif kind == "right":
            ops.append(L([(20 - n, y), (20, y)], c))
        else:
            ops.append(L([(12 - n / 2, y), (12 + n / 2, y)], c))
    return ops


def g_valign(kind, c=INK):
    y0 = {"top": 7.5, "middle": 10.2, "bottom": 13}[kind]
    return [RR(3, 3, 18, 18, 3, stroke=c, sw=1.6), L([(7, y0), (17, y0)], c), L([(7, y0 + 3.6), (14, y0 + 3.6)], c)]


def g_caption(kind):
    if kind == "above":
        return [L([(4, 6), (20, 6)], ACCENT, 2.6), L([(4, 12), (20, 12)], INK), L([(4, 17), (15, 17)], INK)]
    if kind == "below":
        return [L([(4, 6.5), (20, 6.5)], INK), L([(4, 11.5), (15, 11.5)], INK), L([(4, 18), (20, 18)], ACCENT, 2.6)]
    return [RR(2.5, 4, 19, 16, 2.5, stroke=INK, sw=1.6), L([(6, 16.4), (18, 16.4)], ACCENT, 2.4)]   # bottom of screen


def g_T(c=INK):
    return [L([(6, 6), (18, 6)], c, 2.4), L([(12, 6), (12, 18.5)], c, 2.4)]


def g_effect(kind):
    if kind == "none":
        return g_T()
    shadow = G(g_T(INK), opacity=0.45 if kind == "shadow" else 0.7, dx=1.7, dy=1.7,
               blur=0 if kind == "shadow" else 1.1)
    return [shadow] + g_T()


def g_transition(kind):
    frame = RR(3, 5, 18, 14, 2.5, stroke=INK)
    clip = (3, 5, 18, 14, 2.5)
    if kind == "none":
        return [frame]
    if kind == "fade":
        return [G([RR(3, 5, 18, 14, 0, grad=(3, 0, 21, 0, rgba(INK, 0.95), rgba(INK, 0.0)))], clip=clip), frame]
    if kind == "fadeoutin":
        return [G([RR(3, 5, 9, 14, 0, grad=(3, 0, 12, 0, rgba(INK, 0.95), rgba(INK, 0.0))),
                   RR(12, 5, 9, 14, 0, grad=(12, 0, 21, 0, rgba(INK, 0.0), rgba(INK, 0.95)))], clip=clip), frame]
    arrows = {"right": [L([(7, 12), (16.6, 12)]), L([(13.2, 8.6), (16.6, 12), (13.2, 15.4)])],
              "left": [L([(17, 12), (7.4, 12)]), L([(10.8, 8.6), (7.4, 12), (10.8, 15.4)])],
              "up": [L([(12, 16), (12, 8.3)]), L([(8.8, 11.5), (12, 8.3), (15.2, 11.5)])],
              "down": [L([(12, 8), (12, 15.7)]), L([(8.8, 12.5), (12, 15.7), (15.2, 12.5)])]}
    return [frame] + arrows[kind]


def g_flag_frame(stripes, extra=None):
    x, y, w, h, r = 3, 9, 42, 30, 4
    ops = []
    n = len(stripes)
    for i, col in enumerate(stripes):
        ops.append(RR(x, y + h * i / n, w, h / n + 0.5, 0, fill=col))
    ops += extra or []
    return [G(ops, clip=(x, y, w, h, r)), RR(x + 0.5, y + 0.5, w - 1, h - 1, r - 0.5, stroke=(0, 0, 0, 46), sw=1)]


def flag_uk():
    x, y, w, h = 3, 9, 42, 30
    blue, red, white = (1, 33, 105, 255), (200, 16, 46, 255), WHITE
    ops = [RR(x, y, w, h, 0, fill=blue),
           L([(x, y), (x + w, y + h)], white, 6), L([(x + w, y), (x, y + h)], white, 6),
           L([(x, y), (x + w, y + h)], red, 2), L([(x + w, y), (x, y + h)], red, 2),
           RR(x, y + h / 2 - 5, w, 10, 0, fill=white), RR(x + w / 2 - 5, y, 10, h, 0, fill=white),
           RR(x, y + h / 2 - 3, w, 6, 0, fill=red), RR(x + w / 2 - 3, y, 6, h, 0, fill=red)]
    return [G(ops, clip=(x, y, w, h, 4)), RR(x + 0.5, y + 0.5, w - 1, h - 1, 3.5, stroke=(0, 0, 0, 46), sw=1)]


def photo_art(x, y, w, h, r=4):
    """A tiny landscape photo used by the picture-scaling illustrations."""
    sky = RR(x, y, w, h, 0, grad=(0, y, 0, y + h, rgba(ACCENT, 0.55), rgba(ACCENT, 0.25)))
    hill = POLY([(x, y + h), (x + w * 0.35, y + h * 0.45), (x + w * 0.6, y + h * 0.75), (x + w * 0.78, y + h * 0.55),
                 (x + w, y + h * 0.8), (x + w, y + h)], rgba(GREEN, 0.9))
    sun = C(x + w * 0.72, y + h * 0.28, min(w, h) * 0.11, fill=ORANGE)
    return G([sky, hill, sun], clip=(x, y, w, h, r))


def art_fit(kind):
    if kind == "expand_small":            # 128 x 96
        screen = RR(8, 8, 112, 72, 6, stroke=INK, sw=3)
        ops = [photo_art(50, 32, 28, 24, 3)]
        for (ax, ay), (bx, by) in (((46, 28), (22, 18)), ((82, 28), (106, 18)), ((46, 60), (22, 70)),
                                   ((82, 60), (106, 70))):
            ops.append(L([(ax, ay), (bx, by)], ACCENT, 3))
            ang = math.atan2(by - ay, bx - ax)
            for da in (2.5, -2.5):
                ops.append(L([(bx, by), (bx - 7 * math.cos(ang + da / 5), by - 7 * math.sin(ang + da / 5))], ACCENT, 3))
        return ops + [screen]
    if kind == "fit":                     # 128 x 96
        return [G([RR(8, 8, 36, 72, 0, fill=rgba(INK, 0.28)), RR(84, 8, 36, 72, 0, fill=rgba(INK, 0.28))],
                  clip=(8, 8, 112, 72, 6)), photo_art(44, 8, 40, 72, 0), RR(8, 8, 112, 72, 6, stroke=INK, sw=3)]
    # fit by expanding (crop)             128 x 65
    return [G([photo_art(2, 2, 124, 61, 5)], opacity=0.3), photo_art(14, 9, 100, 47, 5),
            RR(14, 9, 100, 47, 5, stroke=INK, sw=3)]


def control_button(kind, state):
    bg = {"normal": 0.42, "hovered": 0.62, "pressed": 0.78}[state]
    r = {"normal": 40, "hovered": 42, "pressed": 37}[state]
    ga = {"normal": 0.88, "hovered": 1.0, "pressed": 0.82}[state]
    s = r / 40
    W = rgba(WHITE, ga)
    ops = [C(48, 48, r, fill=(15, 23, 42, int(255 * bg)))]
    if state == "hovered":
        ops.append(C(48, 48, r - 1.5, stroke=(255, 255, 255, 80), w=2))
    P = lambda x, y: (48 + (x - 48) * s, 48 + (y - 48) * s)
    if kind == "prev":
        ops.append(L([P(54, 32), P(38, 48), P(54, 64)], W, 6 * s))
    elif kind == "next":
        ops.append(L([P(42, 32), P(58, 48), P(42, 64)], W, 6 * s))
    else:
        ops += [L([P(36, 36), P(60, 60)], W, 6 * s), L([P(60, 36), P(36, 60)], W, 6 * s)]
    return ops


def app_icon(small=False):
    """256-unit app icon. `small` simplifies the glyph for 16-32 px (Fluent/HIG guidance)."""
    ops = [RR(8, 8, 240, 240, 56, grad=(8, 8, 248, 248, BRAND_A, BRAND_B)),
           RR(8, 8, 240, 124, 56, grad=(0, 8, 0, 132, (255, 255, 255, 46), (255, 255, 255, 0)))]
    if small:
        return ops + [RR(40, 48, 176, 118, 16, fill=WHITE), L([(128, 166), (128, 196)], WHITE, 20),
                      L([(88, 212), (168, 212)], WHITE, 20)]
    return ops + [G([RR(52, 66, 152, 100, 14, fill=(0, 0, 0, 90))], blur=7),
                  RR(52, 60, 152, 100, 14, fill=WHITE),
                  L([(78, 92), (178, 92)], BRAND_A, 12), L([(78, 114), (160, 114)], rgba(BRAND_A, 0.75), 12),
                  L([(78, 136), (140, 136)], rgba(BRAND_A, 0.5), 12),
                  L([(128, 160), (128, 184)], WHITE, 14), L([(96, 204), (128, 184), (160, 204)], WHITE, 14)]


def fit_text(text, bold, max_w, start):
    size = start
    while size > 8:
        f = load_font(size, bold)
        if f.getlength(text) <= max_w:
            return size
        size -= 1
    return size


def splash():
    ops = [RR(0, 0, 450, 179, 14, grad=(0, 0, 450, 179, (15, 23, 42, 255), (30, 58, 138, 255))),
           G([C(400, 10, 110, fill=(109, 40, 217, 110))], blur=45, clip=(0, 0, 450, 179, 14))]
    ops += xform(app_icon(), 112 / 256, 24, 33.5)
    ts = fit_text("SoftProjector", True, 270, 42)
    ops += [TXT(152, 96, "SoftProjector", ts, WHITE, True),
            TXT(154, 124, "Open-source presentation software", fit_text("Open-source presentation software", False,
                                                                        270, 16), (203, 213, 225, 255))]
    return ops


def about_logo():
    ops = xform(app_icon(), 80 / 256, 6, 19.5)
    ts = fit_text("SoftProjector", True, 196, 34)
    return ops + [TXT(98, 70, "SoftProjector", ts, ACCENT, True)]


def player_surface():
    return [RR(0, 0, 400, 400, 0, grad=(0, 0, 0, 400, (30, 41, 59, 255), (11, 18, 32, 255)))]


def wide_screen():
    return [RR(8, 6, 223, 70, 8, stroke=INK, sw=4), L([(119.5, 78), (119.5, 88)], INK, 4),
            L([(96, 91), (143, 91)], INK, 4)]


# --------------------------------------------------------------------------
# Icon catalogue: name -> (width, height, view, ops, meaning, iOS, Android)
# --------------------------------------------------------------------------
V24 = (24, 24)
ICONS = {}


def add(name, size, ops, meaning, ios, android, view=V24):
    ICONS[name] = (size[0], size[1], view, ops, meaning, ios, android)


# content types (tabs + schedule list markers) - colour-coded
add("book.png", (48, 48), g_book(ACCENT), "Bible tab / schedule item", "book", "menu_book")
add("song_tab.png", (49, 48), g_note(VIOLET), "Songs tab / schedule item", "music.note", "music_note")
add("photo.png", (128, 128), g_photo(GREEN), "Pictures tab / schedule item", "photo", "image")
add("video.png", (128, 128), g_video(ORANGE), "Media tab / schedule item", "video", "videocam")
add("announce.png", (128, 128), g_announce(TEAL), "Announcements tab / schedule item", "megaphone", "campaign")

# per-tab toolbar actions (New / Edit / Copy / Delete)
add("song_new.png", (48, 48), with_badge(g_note(), "plus", ACCENT), "New song", "music.note + plus.circle.fill", "music_note + add_circle")
add("song_edit.png", (48, 48), with_badge(g_note(), "pencil", ACCENT), "Edit song", "music.note + pencil.circle.fill", "music_note + edit")
add("song_copy.png", (48, 48), with_badge(g_note(), "copy", ACCENT), "Copy song to a new songbook", "music.note + doc.on.doc", "music_note + content_copy")
add("song_delete.png", (48, 48), with_badge(g_note(), "x", RED), "Delete song", "music.note + xmark.circle.fill", "music_note + cancel")
add("song_count.png", (48, 48), with_badge(g_note(), "chart", ACCENT), "Song counter (usage statistics)", "music.note + chart.bar.fill", "music_note + leaderboard")
add("announce_new.png", (48, 48), with_badge(g_announce(), "plus", ACCENT), "New announcement", "megaphone + plus.circle.fill", "campaign + add_circle")
add("announce_edit.png", (48, 48), with_badge(g_announce(), "pencil", ACCENT), "Edit announcement", "megaphone + pencil.circle.fill", "campaign + edit")
add("announce_copy.png", (48, 48), with_badge(g_announce(), "copy", ACCENT), "Copy announcement", "megaphone + doc.on.doc", "campaign + content_copy")
add("announce_delete.png", (48, 48), with_badge(g_announce(), "x", RED), "Delete announcement", "megaphone + xmark.circle.fill", "campaign + cancel")
add("slideshow_new.png", (127, 128), with_badge(g_slideshow(), "plus", ACCENT), "New slide show", "play.rectangle + plus.circle.fill", "slideshow + add_circle")
add("slideshow_edit.png", (128, 128), with_badge(g_slideshow(), "pencil", ACCENT), "Edit slide show", "play.rectangle + pencil.circle.fill", "slideshow + edit")
add("slideshow_delete.png", (128, 128), with_badge(g_slideshow(), "x", RED), "Delete slide show", "play.rectangle + xmark.circle.fill", "slideshow + cancel")
add("video_add.png", (128, 128), with_badge(g_video(), "plus", ACCENT), "Add media files", "video.badge.plus", "video_call")
add("video_remove.png", (128, 128), with_badge(g_video(), "minus", RED), "Remove media file from library", "video + minus.circle.fill", "videocam + remove_circle")
add("bibleHistoryDelete.png", (48, 48), with_badge(g_clock(), "x", RED), "Delete item from Bible history", "clock.badge.xmark", "history + cancel")
add("add_songbook.png", (48, 48), with_badge(g_book(), "plus", ACCENT), "Add songbook (currently unused)", "book + plus.circle.fill", "library_add")

# presentation control
add("show.png", (48, 48), g_present(ACCENT, "up"), "Display to the screen (F4)", "rectangle.on.rectangle / tv", "present_to_all")
add("hide.png", (48, 48), g_present(INK, "x"), "Show passive screen (stop displaying)", "xmark.rectangle", "cancel_presentation")
add("clear.png", (48, 48), g_format_clear(), "Clear display text (Shift+Esc)", "eraser / textformat + slash", "format_clear")
add("go_live.png", (48, 48), [C(12, 12, 10, fill=GREEN), POLY([(10.1, 8.1), (16.2, 12), (10.1, 15.9)], WHITE, rnd=1)],
    "Quickly display the selection (Go live)", "play.circle.fill", "play_circle (filled)")
add("display_on.png", (51, 51), g_monitor(ACCENT, filled=True), "Display screen is on (toggle)", "display (filled)", "desktop_windows (filled)")
add("display_off.png", (51, 51), g_monitor(INK) + [ERASE([L([(3.5, 3.5), (20.5, 20.5)], w=5.5)]), L([(4, 4), (20, 20)], INK)],
    "Display screen is off (toggle)", "display + slash", "desktop_access_disabled")
add("display.png", (128, 128), g_monitor(INK), "Display (currently unused)", "display", "desktop_windows")

# schedule
add("scheduleAdd.png", (48, 48), g_list() + [L([(18, 13), (18, 21)], ACCENT), L([(14, 17), (22, 17)], ACCENT)],
    "Add to schedule", "text.badge.plus", "playlist_add")
add("scheduleRemove.png", (48, 48), g_list() + [L([(15.2, 13.2), (20.8, 18.8)], RED), L([(20.8, 13.2), (15.2, 18.8)], RED)],
    "Remove from schedule", "text.badge.xmark", "playlist_remove")
add("scheduleClear.png", (48, 48), g_delete_sweep(RED), "Clear schedule", "trash", "delete_sweep")
add("scheduleNew.png", (48, 48), g_doc_plus(), "Start new schedule", "doc.badge.plus", "note_add")
add("scheduleOpen.png", (48, 48), g_folder_open(), "Open schedule", "folder", "folder_open")
add("scheduleSave.png", (48, 48), g_tray_down(), "Save schedule", "square.and.arrow.down", "download / save_alt")
add("scheduleUp.png", (48, 48), g_arrow(INK, True), "Move schedule item up", "arrow.up", "arrow_upward")
add("scheduleDown.png", (48, 48), g_arrow(INK, False), "Move schedule item down", "arrow.down", "arrow_downward")
add("scheduleUpM.png", (48, 48), g_to_edge(INK, True), "Move schedule item to top", "arrow.up.to.line", "vertical_align_top")
add("scheduleDownM.png", (48, 48), g_to_edge(INK, False), "Move schedule item to bottom", "arrow.down.to.line", "vertical_align_bottom")

# general actions
add("add.png", (48, 48), g_plus(ACCENT), "Add (theme, pictures)", "plus", "add")
add("remove.png", (48, 48), [C(12, 12, 9, stroke=RED), L([(7.8, 12), (16.2, 12)], RED)], "Remove picture", "minus.circle", "remove_circle_outline")
add("remove_all.png", (48, 48), g_trash(RED), "Clear all pictures", "trash", "delete")
add("edit.png", (49, 48), g_pencil(), "Edit (currently unused)", "pencil", "edit")
add("new.png", (48, 48), with_badge([L([(13.5, 3), (6, 3), (6, 21), (18, 21), (18, 7.5)], INK, closed=True),
                                      L([(13.5, 3), (13.5, 7.5), (18, 7.5)], INK)], "plus", ACCENT),
    "New document (currently unused)", "doc.badge.plus", "note_add")
add("open.png", (48, 48), g_folder_open(), "Open (currently unused)", "folder", "folder_open")
add("save.png", (48, 48), g_tray_down(), "Save (currently unused)", "square.and.arrow.down", "download")
add("print.png", (48, 48), g_printer(), "Print", "printer", "print")
add("search.png", (128, 128), g_search(), "Search", "magnifyingglass", "search")
add("settings.png", (49, 49), g_gear(), "Settings", "gearshape", "settings")
add("help.png", (48, 48), g_help(), "Help", "questionmark.circle", "help")
add("database.png", (48, 48), g_database(), "Manage database (import/export)", "cylinder.split.1x2", "database")
add("common.png", (48, 48), g_layers_check(), "Apply background to all active backgrounds", "square.on.square + checkmark", "library_add_check")
add("base-22x22-actions-application-exit.png", (22, 22), g_logout(), "Exit SoftProjector",
    "rectangle.portrait.and.arrow.right", "logout")
add("up.png", (22, 22), g_arrow(INK, True), "Move picture up", "arrow.up", "arrow_upward")
add("down.png", (22, 22), g_arrow(INK, False), "Move picture down", "arrow.down", "arrow_downward")

# media transport (filled, per platform convention)
add("play.png", (128, 128), g_play(), "Play", "play.fill", "play_arrow (filled)")
add("pause.png", (128, 128), g_pause(), "Pause", "pause.fill", "pause (filled)")
add("stop.png", (128, 128), g_stop(), "Stop", "stop.fill", "stop (filled)")
add("speaker.png", (48, 48), g_speaker(), "Sound on (mute toggle)", "speaker.wave.2", "volume_up")
add("speakerMute.png", (48, 48), g_speaker(muted=True), "Sound muted (mute toggle)", "speaker.xmark", "volume_off")

# text layout / effects / transitions (settings; several currently unused)
for k in ("Left", "Center", "Right"):
    add(f"aling{k}.png", (24, 24), g_align(k.lower()), f"Align text {k.lower()} (unused)", f"text.align{k.lower()}",
        f"format_align_{k.lower()}")
for k in ("Top", "Middle", "Bottom"):
    add(f"aling{k}.png", (24, 24), g_valign(k.lower()), f"Align text to {k.lower()} (unused)",
        f"align.vertical.{'center' if k == 'Middle' else k.lower()}", f"vertical_align_{'center' if k == 'Middle' else k.lower()}")
add("aboveText.png", (24, 24), g_caption("above"), "Caption above text (unused)", "rectangle.topthird.inset.filled", "border_top")
add("belowText.png", (24, 24), g_caption("below"), "Caption below text (unused)", "rectangle.bottomthird.inset.filled", "border_bottom")
add("bottomOfScreen.png", (24, 24), g_caption("bottom"), "Caption at bottom of screen (unused)", "captions.bubble", "subtitles")
add("toTopOfScreen.png", (24, 24), g_to_edge(INK, True), "Move to top of screen (unused)", "arrow.up.to.line", "vertical_align_top")
add("toBottomOfScreen.png", (24, 24), g_to_edge(INK, False), "Move to bottom of screen (unused)", "arrow.down.to.line", "vertical_align_bottom")
add("effectsNone.png", (24, 24), g_effect("none"), "Text effect: none (unused)", "textformat", "text_fields")
add("effectsShadow.png", (24, 24), g_effect("shadow"), "Text effect: shadow (unused)", "shadow", "text_fields + shadow")
add("effectsBlurredShadow.png", (24, 24), g_effect("blur"), "Text effect: blurred shadow (unused)", "shadow (soft)", "blur_on")
add("solidColor.png", (24, 24), [RR(4, 4, 16, 16, 4, fill=ACCENT)], "Solid colour background (unused)", "square.fill", "format_color_fill")
for k, lab in (("None", "none"), ("Fade", "fade"), ("FadeOutIn", "fadeoutin"), ("MoveUp", "up"),
               ("MoveDown", "down"), ("MoveLeft", "left"), ("MoveRight", "right")):
    add(f"tran{k}.png", (24, 24), g_transition(lab), f"Transition: {k} (unused)", "rectangle / arrow.*.square",
        "animation / arrow_*")

# QML projector overlay controls (white glyph on translucent 'glass' disc)
for k in ("Prev", "Next", "Exit"):
    for st, suf in (("normal", ""), ("hovered", "Hovered"), ("pressed", "Pressed")):
        add(f"control{k}{suf}.png", (96, 96), control_button(k.lower(), st),
            f"Projector on-screen {k.lower()} button ({st})",
            {"Prev": "chevron.left", "Next": "chevron.right", "Exit": "xmark"}[k] + " (in circle)",
            {"Prev": "chevron_left", "Next": "chevron_right", "Exit": "close"}[k], view=(96, 96))

# illustrations & brand
add("ExpandSmall.png", (128, 96), art_fit("expand_small"), "Picture setting: expand small images", "arrow.up.left.and.arrow.down.right", "open_in_full", view=(128, 96))
add("FitToScreen.png", (128, 96), art_fit("fit"), "Picture setting: fit images to screen", "aspectratio", "fit_screen", view=(128, 96))
add("FitToScreenByExpanding.png", (128, 65), art_fit("crop"), "Picture setting: fit by expanding (crop)", "crop", "crop", view=(128, 65))
add("screen.png", (239, 98), wide_screen(), "Screen illustration (unused)", "display", "tv", view=(239, 98))
add("playerScreen.png", (400, 400), player_surface(), "Media info panel background (stretched by a style sheet)", "-", "surface", view=(400, 400))
add("flag_uk.png", (48, 48), flag_uk(), "Language: English", "(flag)", "(flag)", view=(48, 48))
add("softprojector.png", (256, 256), app_icon(), "Application / window icon", "App Icon (iOS 18+ layered)", "Adaptive icon", view=(256, 256))
add("splash.png", (450, 179), splash(), "Splash screen", "Launch screen", "Splash screen", view=(450, 179))
add("softprojector_cloud.png", (300, 119), about_logo(), "About dialog logo", "-", "-", view=(300, 119))

FLAGS = {
    "flag_cs.png": g_flag_frame([WHITE, (215, 20, 26, 255)],
                                [POLY([(3, 9), (24, 24), (3, 39)], (17, 69, 126, 255))]),
    "flag_de.png": g_flag_frame([(0, 0, 0, 255), (221, 0, 0, 255), (255, 206, 0, 255)]),
    "flag_hy.png": g_flag_frame([(217, 0, 18, 255), (0, 51, 160, 255), (242, 168, 0, 255)]),
    "flag_ru.png": g_flag_frame([WHITE, (0, 57, 166, 255), (213, 43, 30, 255)]),
    "flag_ua.png": g_flag_frame([(0, 87, 183, 255), (255, 215, 0, 255)]),
}


# --------------------------------------------------------------------------
def write(name, w, h, view, ops, out_dir, svg=True):
    rasterize(ops, w, h, view).save(out_dir / name, optimize=True)
    rasterize(ops, w * 2, h * 2, view).save(out_dir / name.replace(".png", "@2x.png"), optimize=True)
    if svg:
        (SVG_DIR / name.replace(".png", ".svg")).write_text(SvgWriter().document(ops, w, h, view))


def main():
    SVG_DIR.mkdir(parents=True, exist_ok=True)
    for name, (w, h, view, ops, *_meta) in ICONS.items():
        write(name, w, h, view, ops, ICON_DIR)
    for name, ops in FLAGS.items():
        write(name, 48, 48, (48, 48), ops, FLAG_DIR, svg=False)
        (SVG_DIR / name.replace(".png", ".svg")).write_text(SvgWriter().document(ops, 48, 48, (48, 48)))

    # Windows icon: every size rendered natively; simplified glyph at <= 32 px
    sizes = [16, 20, 24, 32, 40, 48, 64, 96, 128, 256]
    imgs = [rasterize(app_icon(small=s <= 32), s, s, (256, 256)) for s in sizes]
    imgs[-1].save(SRC / "softprojector.ico", format="ICO", sizes=[(s, s) for s in sizes], append_images=imgs[:-1])

    # Documentation
    rows = []
    for name, (w, h, view, ops, meaning, ios, android) in ICONS.items():
        rows.append(f"| `{name}` | {w}×{h} | {meaning} | {ios} | {android} |")
    pal = "\n".join(f"| {k} | `#{v[0]:02X}{v[1]:02X}{v[2]:02X}` | "
                    + " | ".join(f"{contrast(v, bg):.2f}:1" for bg in BACKGROUNDS.values()) + " |"
                    for k, v in PALETTE.items())
    (ROOT / "ICONS.md").write_text(DOC.format(palette=pal, bgs=" | ".join(BACKGROUNDS), rows="\n".join(rows),
                                              n=len(ICONS), f=len(FLAGS)))
    print(f"{len(ICONS)} icons + {len(FLAGS)} flags (+@2x, +SVG) and softprojector.ico ({len(sizes)} sizes) written")
    worst = min(contrast(v, bg) for v in PALETTE.values() for bg in BACKGROUNDS.values())
    print(f"minimum glyph contrast across both themes: {worst:.2f}:1 (WCAG 2.2 SC 1.4.11 requires 3:1)")


DOC = """# SoftProjector icon system (v3.0)

Generated by `tools/icons/build_icons.py` — edit the script, not the PNGs,
then run `python tools/icons/build_icons.py` from the repository root.
{n} icons and {f} language flags; each has an SVG master in `src/icons/svg/`, a PNG at
its original size and an `@2x` PNG that Qt picks up automatically on high-DPI
screens (QIcon and QML `Image`). File names are unchanged, so no code or
`.ui` references needed editing.

## Design standard

The set follows the conventions shared by Apple **SF Symbols**, Google
**Material Symbols** and Microsoft **Fluent 2**, drawn from scratch (SF Symbols'
licence does not allow use outside Apple platforms; Material Symbols are
Apache-2.0 and could be substituted by name using the mapping below).

* **Grid:** 24 × 24 units, 2-unit safe margin (20-unit live area).
* **Stroke:** 2 units, round caps and joins; corner radius 2–2.5 units.
* **Style:** outlined by default; **filled** for active/"on" states and media
  transport (play / pause / stop), as on iOS and Android.
* **Badges:** 11-unit disc at the bottom-right, cut out of the base glyph with
  a 1.6-unit transparent gap (iOS `*.badge.*` / Material `*_circle` pattern).
* **Colour:** one neutral ink for toolbar glyphs. Colour carries meaning only:
  accent = primary action, red = destructive, green = go live, and one hue per
  content type (Bible, Songs, Pictures, Media, Announcements) so schedule items
  can be told apart at a glance.
* **Contrast on both themes:** Qt does not tint PNG icons per theme, so every
  glyph colour is tuned to relative luminance {target} — it keeps ≥ 3:1 contrast
  (WCAG 2.2 SC 1.4.11) against the light theme *and* the app's dark theme.
* **Small sizes:** the Windows `.ico` is rendered natively at 16–256 px with a
  simplified glyph at ≤ 32 px.

| Colour role | Hex | {bgs} |
|---|---|---|---|---|---|
{palette}

## Icon mapping

| File | Size (@1x) | Meaning / where used | iOS (SF Symbols) analogue | Android (Material Symbols) analogue |
|---|---|---|---|---|
{rows}

Language flags (`src/translations/flag_*.png`) use the same rounded, bordered style.
"""
DOC = DOC.replace("{target}", str(TARGET_L))

if __name__ == "__main__":
    main()
