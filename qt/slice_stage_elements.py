"""Slice the Stage Console element sheets into 9-slice ICO looks.

ChatGPT exported the concept's controls as transparent sheets in
docs/design/futuristic-2026-10-24/elements/: tiles.png and keys.png hold
every tile and key in its own state, rim light and glow but no text, and
panels.png the empty group panels. Each element is cut out with its glow, scaled to the desk, and
reduced to its corners plus a two-pixel middle that the BorderImage
stretches. BMP-encoded ICO because the tablet's Qt reads no PNG. Run after
replacing a sheet or changing a box below.
"""
from pathlib import Path

from PIL import Image, ImageEnhance

# Per sheet: the scale to the desk, how far inside each sheet body the desk's
# edge falls (the sheets draw a dark bezel outside the rim light, where the
# concept puts the rim on the edge), the glow kept outside each body at the
# desk's scale, the corner size, a brightness factor where the sheet's glass or
# light is weaker or stronger than the concept's, the bodies (left, top, right, bottom) with
# the look each becomes, and the sheet's unused bodies, which only keep their
# own rim and glow out of a neighbour's crop.
SHEETS = [
    {
        "file": "tiles.png", "scale": 0.38, "margin": 12, "slice": 22, "inset": 5, "halo": 0.25,
        "looks": [
            ((43, 81, 219, 258), "fill-idle"),
            ((252, 81, 434, 259), "fill-selected"),
            ((472, 81, 658, 258), "fill-hold"),
            ((703, 81, 888, 259), "fill-hold-lit"),
            ((948, 81, 1135, 259), "fill-fog"),
            ((1206, 81, 1395, 259), "fill-fog-lit"),
            ((48, 305, 672, 439), "fill-fog-wide"),
            ((714, 305, 1404, 440), "fill-fog-wide-lit"),
            ((47, 491, 247, 786), "fill-panic"),
            ((950, 566, 1181, 692), "key-tempo"),
            ((1215, 484, 1407, 1003), "fill-master"),
        ],
        "others": [
            (287, 566, 594, 698), (628, 523, 778, 771), (811, 567, 932, 692), (47, 855, 561, 994),
            (593, 856, 1062, 998),
        ],
    },
    {
        "file": "tiles.png", "scale": 0.38, "margin": 12, "slice": 22, "inset": 5, "halo": 0.25, "tone": 0.68,
        "looks": [((287, 566, 594, 698), "fill-quiet")],
        "others": [
            (43, 81, 219, 258), (252, 81, 434, 259), (472, 81, 658, 258), (703, 81, 888, 259),
            (948, 81, 1135, 259), (1206, 81, 1395, 259), (48, 305, 672, 439), (714, 305, 1404, 440),
            (47, 491, 247, 786), (950, 566, 1181, 692), (1215, 484, 1407, 1003),
            (628, 523, 778, 771), (811, 567, 932, 692), (47, 855, 561, 994), (593, 856, 1062, 998),
        ],
    },
    {
        "file": "keys.png", "scale": 0.38, "margin": 12, "slice": 22, "inset": 5, "tone": 1.5,
        "looks": [
            ((963, 126, 1155, 395), "fill-quiet-selected"),
        ],
        "others": [
            (648, 620, 836, 809), (946, 488, 1172, 823), (78, 259, 460, 386), (516, 259, 891, 388), (1242, 259, 1598, 392),
            (88, 646, 546, 781), (1309, 570, 1577, 819),
        ],
    },
    {
        "file": "keys.png", "scale": 0.38, "margin": 10, "slice": 16,
        "looks": [
            ((78, 259, 460, 386), "tab"),
            ((516, 259, 891, 388), "tab-selected"),
        ],
        "others": [
            (963, 126, 1155, 395), (648, 620, 836, 809), (946, 488, 1172, 823),
            (1242, 259, 1598, 392), (88, 646, 546, 781), (1309, 570, 1577, 819),
        ],
    },
    {
        "file": "panels.png", "scale": 0.625, "margin": 8, "slice": 40, "tone": 0.7,
        "looks": [
            ((32, 116, 855, 259), "panel"),
        ],
        "others": [
            (867, 294, 1353, 626), (31, 662, 842, 846), (30, 293, 844, 628), (1375, 280, 1530, 863), (1544, 392, 1626, 698),
            (882, 695, 1158, 819), (1183, 688, 1332, 829),
        ],
    },
    {
        "file": "panels.png", "scale": 0.625, "margin": 12, "slice": 22, "tone": 0.7, "inset": 4,
        "looks": [((884, 114, 1529, 256), "fill-tempo")],
        "others": [
            (32, 116, 855, 259), (867, 294, 1353, 626), (31, 662, 842, 846),
            (30, 293, 844, 628), (1375, 280, 1530, 863), (1544, 392, 1626, 698),
            (882, 695, 1158, 819), (1183, 688, 1332, 829),
        ],
    },
]


def distance(x, y, box):
    left, top, right, bottom = box
    dx = max(left - x, 0, x - right)
    dy = max(top - y, 0, y - bottom)
    return dx * dx + dy * dy


def cut(sheet, body, others, scale, margin, inset):
    # Keep only the pixels nearer this element than any other, so a
    # neighbour's rim or glow never lands in the crop.
    left, top, right, bottom = body
    left, top, right, bottom = left + inset, top + inset, right - inset, bottom - inset
    pad = round(margin / scale)
    piece = sheet.crop((left - pad, top - pad, right + pad, bottom + pad))
    alpha = piece.getchannel("A")
    pixels = alpha.load()
    for y in range(piece.height):
        for x in range(piece.width):
            sx, sy = x + left - pad, y + top - pad
            own = distance(sx, sy, (left, top, right, bottom))
            if own and any(distance(sx, sy, box) < own for box in others):
                pixels[x, y] = 0
    piece.putalpha(alpha)
    size = (round(piece.width * scale), round(piece.height * scale))
    return piece.resize(size, Image.LANCZOS)


def dim_halo(image, margin, halo):
    # Scale the glow beyond the tile's bezel; the sheets spill more light into
    # the gaps than the concept, whose gaps stay nearly black.
    alpha = image.getchannel("A")
    pixels = alpha.load()
    bezel = 2
    reach = margin - bezel
    for y in range(image.height):
        for x in range(image.width):
            edge = min(x, y, image.width - 1 - x, image.height - 1 - y)
            if edge < reach:
                pixels[x, y] = round(pixels[x, y] * halo)
    image.putalpha(alpha)


def fade_edges(image):
    # The sheet's glow runs past the margin; fade it out instead of clipping.
    alpha = image.getchannel("A")
    pixels = alpha.load()
    ramp = 4
    for y in range(image.height):
        for x in range(image.width):
            edge = min(x, y, image.width - 1 - x, image.height - 1 - y)
            if edge < ramp:
                pixels[x, y] = pixels[x, y] * (edge + 1) // (ramp + 1)
    image.putalpha(alpha)


def retone(image, tone):
    alpha = image.getchannel("A")
    image = ImageEnhance.Brightness(image.convert("RGB")).enhance(tone).convert("RGBA")
    image.putalpha(alpha)
    return image


def compact(image, corner):
    # The stretched middle is the average of the element's whole middle span,
    # not one column of it, so its glass shading does not band.
    side = 2 * corner + 2
    out = Image.new("RGBA", (side, side))
    middle_w = image.width - 2 * corner
    middle_h = image.height - 2 * corner
    xs = [(0, 0, corner, corner), (corner, corner, middle_w, 2), (corner + 2, image.width - corner, corner, corner)]
    ys = [(0, 0, corner, corner), (corner, corner, middle_h, 2), (corner + 2, image.height - corner, corner, corner)]
    for dx, sx, sw, w in xs:
        for dy, sy, sh, h in ys:
            block = image.crop((sx, sy, sx + sw, sy + sh)).resize((w, h), Image.BOX)
            out.paste(block, (dx, dy))
    return out


def main():
    here = Path(__file__).resolve().parent
    elements = here.parent / "docs/design/futuristic-2026-10-24/elements"
    target = here / "qml/chrome"
    for group in SHEETS:
        sheet = Image.open(elements / group["file"]).convert("RGBA")
        bodies = [box for box, _ in group["looks"]] + group["others"]
        for body, name in group["looks"]:
            others = [box for box in bodies if box != body]
            piece = cut(sheet, body, others, group["scale"], group["margin"], group.get("inset", 0))
            dim_halo(piece, group["margin"], group.get("halo", 1))
            fade_edges(piece)
            piece = retone(piece, group.get("tone", 1))
            image = compact(piece, group["margin"] + group["slice"])
            image.save(target / f"{name}.ico", sizes=[image.size], bitmap_format="bmp")
            print(name)


if __name__ == "__main__":
    main()
