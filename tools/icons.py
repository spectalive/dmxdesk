#!/usr/bin/env python3
"""The desk's icons, drawn as vectors at 4x and rasterised into 8-bit alpha
bitmaps the painter blends in any colour. Regenerate `src/icon_data.h`:

    python3 tools/icons.py > src/icon_data.h

An 18 px optical square leaves 3 px breathing room inside the 24 px grid;
a 2 px stroke stays legible on the tablet without overpowering the text.
The gear's dense disc is 10% smaller (16.2 px); the short battery is 2 px
wider (20 px including its terminal) and 12 px tall to balance their weight.
The Wi-Fi dot and three arcs share a centre and 4.5 px radial spacing, with
a 70 degree sweep and round ends. BOX downsampling averages the 4x coverage
without a ringing halo, so nonzero alpha stays within one pixel of the optical
box even for the wider battery. All positions at call sites stay unchanged.
"""
import math

from PIL import Image, ImageDraw

S = 4  # supersample
OPTICAL_SIZE = 18
STROKE = 2


def canvas(w, h):
    im = Image.new("L", (w * S, h * S), 0)
    return im, ImageDraw.Draw(im)


def gear(size=24):
    im, d = canvas(size, size)
    cx = cy = size / 2
    r = OPTICAL_SIZE * 0.9 / 2
    pts = []
    for tooth in range(8):
        for angle, radius in [(-22.5, r - STROKE), (-10, r),
                              (10, r), (22.5, r - STROKE)]:
            a = math.radians(tooth * 45 + angle)
            pts.append(((cx + math.cos(a) * radius) * S,
                        (cy + math.sin(a) * radius) * S))
    d.polygon(pts, fill=255)
    hole = r - 2 * STROKE
    d.ellipse([(cx - hole) * S, (cy - hole) * S,
               (cx + hole) * S, (cy + hole) * S], fill=0)
    return im


def lock(size=24, open_=False):
    im, d = canvas(size, size)
    cx = cy = size / 2
    top = cy - OPTICAL_SIZE / 2 + STROKE / 2
    bottom = cy + OPTICAL_SIZE / 2 - STROKE / 2
    half = OPTICAL_SIZE / 3
    body_y = cy - STROKE / 2
    d.rounded_rectangle([(cx - half) * S, body_y * S,
                         (cx + half) * S, bottom * S],
                        radius=STROKE * S, outline=255, width=STROKE * S)
    sh = OPTICAL_SIZE / 2
    x0 = cx - sh / 2 - (STROKE if open_ else 0)
    x1 = x0 + sh
    d.arc([x0 * S, top * S, x1 * S, (top + sh) * S],
          180, 360, fill=255, width=STROKE * S)
    middle = top + sh / 2
    d.line([x0 * S, middle * S, x0 * S, body_y * S],
           fill=255, width=STROKE * S)
    if not open_:
        d.line([(x1 - STROKE / 2) * S, middle * S,
                (x1 - STROKE / 2) * S, body_y * S],
               fill=255, width=STROKE * S)
    return im


def wifi_arc(size=24, index=0):
    """One separately lit fan layer, balanced against the battery and lock.

    A 4 px filled apex replaces the speck. Rings at 5, 9 and 13 px leave
    the same 2 px radial gap from the apex and between strokes. Raising the
    common centre 2 px puts the fan's lower edge beside the battery's.
    """
    im, d = canvas(size, size)
    cx, cy = size / 2, size / 2 + OPTICAL_SIZE / 2 - 2 * STROKE
    radius = STROKE / 2
    if index == 0:
        # Pillow includes both ellipse endpoints; subtract half a sample so
        # the nominal 4 px diameter covers exactly sixteen high-res samples.
        radius = STROKE - 1 / (2 * S)
        d.ellipse([(cx - radius) * S, (cy - radius) * S,
                   (cx + radius) * S, (cy + radius) * S], fill=255)
        return im
    rr = STROKE / 2 + index * (OPTICAL_SIZE - 3 * STROKE) / 3
    pts = []
    for degree in range(235, 306):
        angle = math.radians(degree)
        pts.append(((cx + rr * math.cos(angle)) * S,
                    (cy + rr * math.sin(angle)) * S))
    d.line(pts, fill=255, width=STROKE * S, joint="curve")
    for x, y in [pts[0], pts[-1]]:
        d.ellipse([x - radius * S, y - radius * S,
                   x + radius * S, y + radius * S], fill=255)
    return im


def battery_shell(size=24):
    im, d = canvas(size, size)
    w, h = OPTICAL_SIZE + STROKE, OPTICAL_SIZE * 2 / 3
    x0, y0 = (size - w) / 2, (size - h) / 2
    x1 = x0 + w - STROKE
    d.rounded_rectangle([x0 * S, y0 * S, x1 * S, (y0 + h) * S],
                        radius=STROKE * S, outline=255, width=STROKE * S)
    d.rounded_rectangle([x1 * S, (y0 + h / 3) * S,
                         (x1 + STROKE) * S, (y0 + 2 * h / 3) * S],
                        radius=STROKE * S // 2, fill=255)
    return im


def battery_cell(size=24):
    """The inside of the shell, a full cell; the painter clips it by level."""
    im, d = canvas(size, size)
    w, h = OPTICAL_SIZE + STROKE, OPTICAL_SIZE * 2 / 3
    x0, y0 = (size - w) / 2, (size - h) / 2
    inset = STROKE * 1.5
    d.rounded_rectangle([(x0 + inset) * S, (y0 + inset) * S,
                         (x0 + w - STROKE - inset) * S, (y0 + h - inset) * S],
                        radius=STROKE * S // 2, fill=255)
    return im


def bolt(size=24):
    im, d = canvas(size, size)
    cx, cy, r = size / 2, size / 2, OPTICAL_SIZE / 2
    pts = [(cx + STROKE, cy - r), (cx - r / 2, cy + STROKE / 2),
           (cx, cy + STROKE / 2), (cx - STROKE, cy + r),
           (cx + r / 2, cy - STROKE / 2), (cx, cy - STROKE / 2)]
    d.polygon([(px * S, py * S) for px, py in pts], fill=255)
    return im


def check(size=24):
    im, d = canvas(size, size)
    cx, cy = size / 2, size / 2
    r = (OPTICAL_SIZE - STROKE) / 2
    d.line([(cx - r) * S, cy * S, (cx - STROKE) * S, (cy + r / 2) * S,
            (cx + r) * S, (cy - r / 2) * S],
           fill=255, width=STROKE * S, joint="curve")
    return im


def chevron(size=24):
    im, d = canvas(size, size)
    cx, cy = size / 2, size / 2
    r = (OPTICAL_SIZE - STROKE) / 2
    d.line([(cx - r / 3) * S, (cy - r) * S, (cx + r / 3) * S, cy * S,
            (cx - r / 3) * S, (cy + r) * S],
           fill=255, width=STROKE * S, joint="curve")
    return im


# ---- the show's marks: one per glyph `qlctool deskmap` writes in a
# control's "icon" field (its catalogue in qlctool/glyph.py and
# pick_marks.py). Same 18 px optical square and 2 px stroke as the status
# icons, so a tile's mark weighs what the bolt on a hit does.

def disc(d, cx, cy, r, fill=255):
    d.ellipse([(cx - r) * S, (cy - r) * S, (cx + r) * S, (cy + r) * S], fill=fill)


def ring(d, cx, cy, r):
    """A 2 px circle whose outer edge is r."""
    d.ellipse([(cx - r) * S, (cy - r) * S, (cx + r) * S, (cy + r) * S],
              outline=255, width=STROKE * S)


def stroke(d, pts):
    d.line([(x * S, y * S) for x, y in pts], fill=255, width=STROKE * S,
           joint="curve")
    for x, y in (pts[0], pts[-1]):
        disc(d, x, y, STROKE / 2)


def play(size=24):
    im, d = canvas(size, size)
    cx, cy, r = size / 2, size / 2, OPTICAL_SIZE / 2
    pts = [(cx - r * 0.6, cy - r * 0.95), (cx + r * 0.95, cy),
           (cx - r * 0.6, cy + r * 0.95)]
    d.polygon([(x * S, y * S) for x, y in pts], fill=255)
    return im


def stop(size=24):
    im, d = canvas(size, size)
    cx, cy, h = size / 2, size / 2, OPTICAL_SIZE * 0.8 / 2
    d.rounded_rectangle([(cx - h) * S, (cy - h) * S, (cx + h) * S, (cy + h) * S],
                        radius=STROKE * S, fill=255)
    return im


def microphone(size=24):
    im, d = canvas(size, size)
    cx, top = size / 2, size / 2 - OPTICAL_SIZE / 2
    d.rounded_rectangle([(cx - 3.5) * S, top * S, (cx + 3.5) * S, (top + 11) * S],
                        radius=3.5 * S, fill=255)
    d.arc([(cx - 6.5) * S, (top + 3) * S, (cx + 6.5) * S, (top + 14) * S],
          0, 180, fill=255, width=STROKE * S)
    stroke(d, [(cx, top + 14), (cx, top + 17)])
    stroke(d, [(cx - 4, top + 17), (cx + 4, top + 17)])
    return im


def moon(size=24):
    im, d = canvas(size, size)
    cx, cy, r = size / 2, size / 2, OPTICAL_SIZE / 2 - 0.5
    disc(d, cx, cy, r)
    disc(d, cx + 4.5, cy - 3.5, r - 1.5, fill=0)
    return im


def party(size=24):
    """A popper: the cone from the lower left, its burst to the upper right."""
    im, d = canvas(size, size)
    lo = size / 2 - OPTICAL_SIZE / 2
    hi = size / 2 + OPTICAL_SIZE / 2
    pts = [(lo + 0.5, hi - 0.5), (lo + 4.5, lo + 6), (lo + 12, hi - 5.5)]
    d.polygon([(x * S, y * S) for x, y in pts], fill=255)
    stroke(d, [(lo + 10, lo + 6.5), (lo + 12.5, lo + 1.5)])
    stroke(d, [(lo + 12, lo + 9), (hi - 1.5, lo + 6.5)])
    disc(d, lo + 7, lo + 1.5, 1.4)
    disc(d, hi - 1.5, lo + 1.5, 1.4)
    disc(d, hi - 1.5, lo + 12, 1.4)
    return im


def flame(size=24):
    im, d = canvas(size, size)
    cx, bottom = size / 2, size / 2 + OPTICAL_SIZE / 2
    def drop(r, tip, fill):
        cy = bottom - r
        pts = []
        for i in range(0, 181):
            a = math.radians(i)
            pts.append((cx + r * math.cos(a), cy + r * math.sin(a)))
        # Up from the left of the bowl to the tip, leaning a little, and down.
        pts += [(cx - r, cy - r * 0.6), (cx - r * 0.3, tip + r * 0.5), (cx + r * 0.1, tip),
                (cx + r * 0.6, cy - r * 1.0), (cx + r, cy - r * 0.2)]
        d.polygon([(x * S, y * S) for x, y in pts], fill=fill)
    drop(7, size / 2 - OPTICAL_SIZE / 2, 255)
    drop(3.2, size / 2 + 1, 0)
    return im


def bulb(size=24):
    im, d = canvas(size, size)
    cx, top = size / 2, size / 2 - OPTICAL_SIZE / 2
    ring(d, cx, top + 6.5, 6.5)
    d.rectangle([(cx - 3.5) * S, (top + 10) * S, (cx + 3.5) * S, (top + 13) * S], fill=0)
    stroke(d, [(cx - 3.5, top + 10.5), (cx - 3.5, top + 13)])
    stroke(d, [(cx + 3.5, top + 10.5), (cx + 3.5, top + 13)])
    stroke(d, [(cx - 3.5, top + 14), (cx + 3.5, top + 14)])
    stroke(d, [(cx - 2, top + 17), (cx + 2, top + 17)])
    return im


def palette(size=24):
    im, d = canvas(size, size)
    cx, cy, r = size / 2, size / 2, OPTICAL_SIZE / 2
    ring(d, cx, cy, r)
    disc(d, cx - 3.5, cy - 2.5, 1.6)
    disc(d, cx + 0.5, cy - 4.5, 1.6)
    disc(d, cx + 4.5, cy - 1.5, 1.6)
    disc(d, cx - 1, cy + 4, 2.2)
    return im


def cloud(size=24):
    im, d = canvas(size, size)
    cx, cy = size / 2, size / 2 + 1
    disc(d, cx - 4.5, cy + 1.5, 4)
    disc(d, cx + 0.5, cy - 2, 5.5)
    disc(d, cx + 5, cy + 2, 3.5)
    d.rectangle([(cx - 4.5) * S, (cy + 1.5) * S, (cx + 5) * S, (cy + 5.5) * S], fill=255)
    return im


def arrow_up(size=24):
    im, d = canvas(size, size)
    cx, top = size / 2, size / 2 - OPTICAL_SIZE / 2 + 1
    bottom = size / 2 + OPTICAL_SIZE / 2 - 1
    pts = [(cx, top), (cx + 7.5, top + 8), (cx + 3, top + 8), (cx + 3, bottom),
           (cx - 3, bottom), (cx - 3, top + 8), (cx - 7.5, top + 8)]
    d.polygon([(x * S, y * S) for x, y in pts], outline=255, width=STROKE * S)
    return im


def asterisk(size=24):
    im, d = canvas(size, size)
    cx, cy, r = size / 2, size / 2, OPTICAL_SIZE / 2 - 1
    for k in range(4):
        a = math.radians(k * 45)
        stroke(d, [(cx - r * math.cos(a), cy - r * math.sin(a)),
                   (cx + r * math.cos(a), cy + r * math.sin(a))])
    return im


def arrow_both(size=24):
    im, d = canvas(size, size)
    cx, cy, r = size / 2, size / 2, OPTICAL_SIZE / 2 - 1
    stroke(d, [(cx - r, cy), (cx + r, cy)])
    stroke(d, [(cx - r + 4.5, cy - 4.5), (cx - r, cy), (cx - r + 4.5, cy + 4.5)])
    stroke(d, [(cx + r - 4.5, cy - 4.5), (cx + r, cy), (cx + r - 4.5, cy + 4.5)])
    return im


def target(size=24):
    im, d = canvas(size, size)
    cx, cy = size / 2, size / 2
    ring(d, cx, cy, OPTICAL_SIZE / 2)
    disc(d, cx, cy, 3)
    return im


def gobo(size=24):
    """A gobo's wheel of holes round a centre."""
    im, d = canvas(size, size)
    cx, cy = size / 2, size / 2
    disc(d, cx, cy, 2.5)
    for k in range(8):
        a = math.radians(k * 45)
        disc(d, cx + 6.8 * math.cos(a), cy + 6.8 * math.sin(a), 2.1)
    return im


def sparkle(size=24):
    im, d = canvas(size, size)
    cx, cy, r = size / 2, size / 2, OPTICAL_SIZE / 2
    pts = []
    for k in range(8):
        a = math.radians(k * 45 - 90)
        rr = r if k % 2 == 0 else r * 0.3
        pts.append(((cx + rr * math.cos(a)) * S, (cy + rr * math.sin(a)) * S))
    d.polygon(pts, fill=255)
    return im


def rainbow(size=24):
    im, d = canvas(size, size)
    cx, cy = size / 2, size / 2 + OPTICAL_SIZE / 4
    for r in (OPTICAL_SIZE / 2, OPTICAL_SIZE / 2 - 2 * STROKE + 0.5,
              OPTICAL_SIZE / 2 - 4 * STROKE + 1):
        d.arc([(cx - r) * S, (cy - r) * S, (cx + r) * S, (cy + r) * S],
              180, 360, fill=255, width=STROKE * S)
    return im


def half(size=24, start=90):
    """A ring with one half filled; `start` is Pillow's angle of the fill."""
    im, d = canvas(size, size)
    cx, cy, r = size / 2, size / 2, OPTICAL_SIZE / 2
    ring(d, cx, cy, r)
    d.pieslice([(cx - r) * S, (cy - r) * S, (cx + r) * S, (cy + r) * S],
               start, start + 180, fill=255)
    return im


def grid(size=24):
    im, d = canvas(size, size)
    lo = size / 2 - OPTICAL_SIZE / 2 + 0.5
    hi = size / 2 + OPTICAL_SIZE / 2 - 0.5
    d.rounded_rectangle([lo * S, lo * S, hi * S, hi * S], radius=STROKE * S,
                        outline=255, width=STROKE * S)
    third = (hi - lo) / 3
    for k in (1, 2):
        at = lo + k * third
        d.line([at * S, lo * S, at * S, hi * S], fill=255, width=STROKE * S)
        d.line([lo * S, at * S, hi * S, at * S], fill=255, width=STROKE * S)
    return im


# EFECTOS' families that have no map glyph of their own: a moving head, the
# prism, and the faders of CONTROL.

def moving_head(size=24):
    """A moving head: the lens on its yoke over a base."""
    im, d = canvas(size, size)
    cx, top = size / 2, size / 2 - OPTICAL_SIZE / 2
    bottom = size / 2 + OPTICAL_SIZE / 2
    d.rounded_rectangle([(cx - 5) * S, (top + 1) * S, (cx + 5) * S, (top + 12) * S],
                        radius=4 * S, outline=255, width=STROKE * S)
    stroke(d, [(cx - 8, top + 5), (cx - 8, bottom - 3), (cx + 8, bottom - 3),
               (cx + 8, top + 5)])
    stroke(d, [(cx - 6, bottom - 1), (cx + 6, bottom - 1)])
    return im


def prism(size=24):
    """A triangle outline, the prism's own shape."""
    im, d = canvas(size, size)
    cx, cy, r = size / 2, size / 2, OPTICAL_SIZE / 2
    pts = [(cx, cy - r + 0.5), (cx + r - 0.5, cy + r - 2), (cx - r + 0.5, cy + r - 2)]
    d.polygon([(x * S, y * S) for x, y in pts], outline=255, width=STROKE * S)
    return im


def sliders(size=24):
    """Three faders, each with its knob at another height."""
    im, d = canvas(size, size)
    lo = size / 2 - OPTICAL_SIZE / 2 + 1
    hi = size / 2 + OPTICAL_SIZE / 2 - 1
    for k, at in enumerate((0.3, 0.7, 0.45)):
        y = lo + 2 + k * (hi - lo - 4) / 2
        stroke(d, [(lo, y), (hi, y)])
        x = lo + at * (hi - lo)
        disc(d, x, y, 2.6)
    return im


# ---- the heads' trajectories: one 40 px mark per figure qlctool's EFX
# generator names (its locales' shape_* words), drawn as the path the beam
# travels with a 2.5 px stroke. Each figure comes three ways: alone; the
# "Simultaneo" variant, two copies overlapped on a diagonal; the "Alternado"
# variant, two smaller copies side by side, the right one mirrored.

LARGE = 40
TRAJ_STROKE = 2.5
TRAJ_R = 15  # the figure's half extent inside the 40 px square


def figure_circle():
    pts = [(math.cos(math.radians(a)), math.sin(math.radians(a))) for a in range(0, 361, 4)]
    return [pts], [(1.0, 0.0, 90)]


def figure_eight():
    pts = [(math.cos(t), 0.55 * math.sin(2 * t)) for t in
           (math.radians(a) for a in range(0, 361, 3))]
    return [pts], []


def figure_line():
    return [[(-1.0, 0.0), (1.0, 0.0)]], [(1.0, 0.0, 0), (-1.0, 0.0, 180)]


def figure_diamond():
    pts = [(0.0, -1.0), (1.0, 0.0), (0.0, 1.0), (-1.0, 0.0), (0.0, -1.0)]
    return [pts], [(0.5, -0.5, 45)]


def figure_square():
    k = 0.8
    pts = [(-k, -k), (k, -k), (k, k), (-k, k), (-k, -k)]
    return [pts], [(k, 0.0, 90)]


def figure_leaf():
    """An almond standing on its points: two arcs meeting top and bottom,
    each of radius 1.25 round a centre 0.75 to the other side."""
    left, right = [], []
    span = math.degrees(math.atan2(1.0, 0.75))
    for k in range(0, 41):
        t = math.radians(-span + 2 * span * k / 40)
        x, y = -0.75 + 1.25 * math.cos(t), 1.25 * math.sin(t)
        right.append((x, y))
        left.append((-x, -y))
    return [right, left], [(0.5, 0.0, 90)]


def figure_lissajous():
    """The 1:2 figure turned on its side and opened by a quarter phase:
    two wings crossing in the middle, the butterfly a beam draws."""
    pts = [(math.sin(t), 0.8 * math.sin(2 * t + math.pi / 4)) for t in
           (math.radians(a) for a in range(0, 361, 2))]
    return [pts], []


FIGURES = [
    ("CIRCLE", figure_circle),
    ("EIGHT", figure_eight),
    ("LINE", figure_line),
    ("DIAMOND", figure_diamond),
    ("SQUARE", figure_square),
    ("LEAF", figure_leaf),
    ("LISSAJOUS", figure_lissajous),
]


def draw_figure(d, figure, cx, cy, r, mirror=False):
    paths, arrows = figure()
    sx = -r if mirror else r
    w = TRAJ_STROKE * S
    for path in paths:
        pts = [((cx + x * sx) * S, (cy + y * r) * S) for x, y in path]
        d.line(pts, fill=255, width=round(w), joint="curve")
        for x, y in (pts[0], pts[-1]):
            d.ellipse([x - w / 2, y - w / 2, x + w / 2, y + w / 2], fill=255)
    head = max(3.0, 5.0 * r / TRAJ_R)
    for x, y, angle in arrows:
        if mirror:
            angle = 180 - angle
        ax, ay = cx + x * sx, cy + y * r
        a = math.radians(angle)
        for side in (150, -150):
            b = a + math.radians(side)
            tip = (ax * S, ay * S)
            end = ((ax + head * math.cos(b)) * S, (ay + head * math.sin(b)) * S)
            d.line([tip, end], fill=255, width=round(w))
            d.ellipse([end[0] - w / 2, end[1] - w / 2, end[0] + w / 2, end[1] + w / 2], fill=255)
        d.ellipse([ax * S - w / 2, ay * S - w / 2, ax * S + w / 2, ay * S + w / 2], fill=255)


def trajectory(figure, variant):
    im, d = canvas(LARGE, LARGE)
    c = LARGE / 2
    if variant == "":
        draw_figure(d, figure, c, c, TRAJ_R)
    elif variant == "SIMULTANEOUS":
        r = TRAJ_R * 0.72
        draw_figure(d, figure, c - 4, c - 4, r)
        draw_figure(d, figure, c + 4, c + 4, r)
    else:
        r = TRAJ_R * 0.58
        draw_figure(d, figure, c - 9, c, r)
        draw_figure(d, figure, c + 9, c, r, mirror=True)
    return im


LARGE_ICONS = []
for _name, _figure in FIGURES:
    for _variant in ("", "SIMULTANEOUS", "ALTERNATE"):
        LARGE_ICONS.append(("ICON_TRAJ_" + _name + ("_" + _variant if _variant else ""),
                            trajectory(_figure, _variant)))


ICONS = [
    ("ICON_GEAR", gear()),
    ("ICON_LOCK", lock()),
    ("ICON_LOCK_OPEN", lock(open_=True)),
    ("ICON_WIFI_0", wifi_arc(index=0)),
    ("ICON_WIFI_1", wifi_arc(index=1)),
    ("ICON_WIFI_2", wifi_arc(index=2)),
    ("ICON_WIFI_3", wifi_arc(index=3)),
    ("ICON_BATTERY", battery_shell()),
    ("ICON_BATTERY_CELL", battery_cell()),
    ("ICON_BOLT", bolt()),
    ("ICON_CHECK", check()),
    ("ICON_CHEVRON", chevron()),
    ("ICON_PLAY", play()),
    ("ICON_STOP", stop()),
    ("ICON_MICROPHONE", microphone()),
    ("ICON_MOON", moon()),
    ("ICON_PARTY", party()),
    ("ICON_FLAME", flame()),
    ("ICON_BULB", bulb()),
    ("ICON_PALETTE", palette()),
    ("ICON_CLOUD", cloud()),
    ("ICON_ARROW_UP", arrow_up()),
    ("ICON_ASTERISK", asterisk()),
    ("ICON_ARROW_BOTH", arrow_both()),
    ("ICON_TARGET", target()),
    ("ICON_GOBO", gobo()),
    ("ICON_SPARKLE", sparkle()),
    ("ICON_RAINBOW", rainbow()),
    ("ICON_HALF_LEFT", half(start=90)),
    ("ICON_HALF_RIGHT", half(start=270)),
    ("ICON_HALF_BOTTOM", half(start=0)),
    ("ICON_HALF_TOP", half(start=180)),
    ("ICON_GRID", grid()),
    ("ICON_MOVING_HEAD", moving_head()),
    ("ICON_PRISM", prism()),
    ("ICON_SLIDERS", sliders()),
]

# The map's glyph for each mark, as UTF-8. The bolt the hits already wear
# is the flash's own.
GLYPHS = [
    ("\u25b6", "ICON_PLAY"),
    ("\u23f9", "ICON_STOP"),
    ("\U0001f3a4", "ICON_MICROPHONE"),
    ("\U0001f319", "ICON_MOON"),
    ("\U0001f389", "ICON_PARTY"),
    ("\U0001f525", "ICON_FLAME"),
    ("\U0001f4a1", "ICON_BULB"),
    ("\u26a1", "ICON_BOLT"),
    ("\U0001f3a8", "ICON_PALETTE"),
    ("\u2601", "ICON_CLOUD"),
    ("\u21e7", "ICON_ARROW_UP"),
    ("\u2733", "ICON_ASTERISK"),
    ("\u2194", "ICON_ARROW_BOTH"),
    ("\u25ce", "ICON_TARGET"),
    ("\u274b", "ICON_GOBO"),
    ("\u2727", "ICON_SPARKLE"),
    ("\U0001f308", "ICON_RAINBOW"),
    ("\u25d0", "ICON_HALF_LEFT"),
    ("\u25d1", "ICON_HALF_RIGHT"),
    ("\u25d2", "ICON_HALF_BOTTOM"),
    ("\u25d3", "ICON_HALF_TOP"),
    ("\u25a6", "ICON_GRID"),
]


def emit():
    print("// Generated by tools/icons.py: 24 px icons as 8-bit alpha, row-major.")
    print("// Do not edit; change the vectors and regenerate.")
    print("#ifndef ICON_DATA_H\n#define ICON_DATA_H\n#include <stdint.h>\n")
    print("#define ICON_SIZE 24")
    print("enum icon_id {")
    for name, _ in ICONS:
        print(f"    {name},")
    print("    ICON_COUNT\n};\n")
    print("static const uint8_t ICON_ALPHA[ICON_COUNT][ICON_SIZE * ICON_SIZE] = {")
    for name, im in ICONS:
        small = im.resize((im.width // S, im.height // S), Image.Resampling.BOX)
        data = list(small.tobytes())
        print(f"    [{name}] = {{")
        for row in range(24):
            vals = ", ".join(f"{v:3d}" for v in data[row * 24:(row + 1) * 24])
            print(f"        {vals},")
        print("    },")
    print("};\n")
    print("// The map's \"icon\" glyphs, as UTF-8, and the mark each one draws.")
    print("static const struct {\n    const char *glyph;\n    enum icon_id id;\n} ICON_GLYPHS[] = {")
    for glyph, name in GLYPHS:
        utf8 = "".join(f"\\x{b:02x}" for b in glyph.encode())
        print(f'    {{ "{utf8}", {name} }},   // U+{ord(glyph):04X}')
    print("};\n")
    print("// The heads' trajectories, 40 px, the same alpha and order.")
    print(f"#define ICON_LARGE_SIZE {LARGE}")
    print("enum icon_large_id {")
    for name, _ in LARGE_ICONS:
        print(f"    {name},")
    print("    ICON_LARGE_COUNT\n};\n")
    print("static const uint8_t ICON_LARGE_ALPHA[ICON_LARGE_COUNT][ICON_LARGE_SIZE * ICON_LARGE_SIZE] = {")
    for name, im in LARGE_ICONS:
        small = im.resize((LARGE, LARGE), Image.Resampling.BOX)
        data = list(small.tobytes())
        print(f"    [{name}] = {{")
        for row in range(LARGE):
            vals = ", ".join(f"{v:3d}" for v in data[row * LARGE:(row + 1) * LARGE])
            print(f"        {vals},")
        print("    },")
    print("};\n\n#endif")


if __name__ == "__main__":
    emit()
