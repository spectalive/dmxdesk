"""Write a soft white halo for each Qt icon into qml/icons/glow/.

Each halo is the icon's shape blurred onto a canvas 1.6 times larger, so the
QML can centre it under the icon without clipping. Like the icons, the halos
are BMP-encoded ICO files, which the tablet's Qt can read.
"""
from pathlib import Path

from PIL import Image, ImageFilter

SCALE = 1.6
OPACITY = 0.4


def main():
    icons = Path(__file__).resolve().parent / "qml/icons"
    glows = icons / "glow"
    glows.mkdir(exist_ok=True)
    for png in sorted(icons.glob("*.png")):
        icon = Image.open(png).convert("RGBA")
        size = round(icon.width * SCALE)
        shape = Image.new("L", (size, size), 0)
        offset = (size - icon.width) // 2
        shape.paste(icon.getchannel("A"), (offset, offset))
        shape = shape.filter(ImageFilter.GaussianBlur(icon.width * 0.09))
        shape = shape.point(lambda value: round(value * OPACITY))
        halo = Image.new("RGBA", (size, size), (235, 242, 255, 0))
        halo.putalpha(shape)
        halo.save(glows / f"{png.stem}.ico", sizes=[halo.size], bitmap_format="bmp")


if __name__ == "__main__":
    main()
