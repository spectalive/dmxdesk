"""Write each Qt icon PNG as a BMP-encoded ICO next to it.

The tablet's Qt build has no PNG reader, only the ICO plugin, and an ICO
whose entries are PNG-compressed would need that reader again. The QML
loads the .ico files on every platform; the PNGs remain the editable source.
"""
from pathlib import Path

from PIL import Image


def main():
    icons = Path(__file__).resolve().parent / "qml/icons"
    for png in sorted(icons.glob("*.png")):
        image = Image.open(png).convert("RGBA")
        image.save(png.with_suffix(".ico"), sizes=[image.size], bitmap_format="bmp")


if __name__ == "__main__":
    main()
