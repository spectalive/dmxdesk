"""Bake the Stage Console floor light into a small ICO stretched over the window.

The floor is lit faintly from the left. On the tablet's GPU a full-window
gradient Rectangle cost about 12 ms a frame while a stretched image costs
next to nothing, so the gradient is baked: 256 px across, stretched to the
window width. BMP-encoded ICO because the tablet's Qt reads no PNG.
"""
from pathlib import Path

from PIL import Image

WIDTH = 256
HEIGHT = 4
STOPS = [(0.0, (0x18, 0x1c, 0x26)), (0.45, (0x0d, 0x11, 0x19)), (1.0, (0x09, 0x0c, 0x15))]


def colour_at(position):
    for (start, low), (end, high) in zip(STOPS, STOPS[1:]):
        if position <= end:
            t = (position - start) / (end - start)
            return tuple(round(a + (b - a) * t) for a, b in zip(low, high)) + (255,)
    return STOPS[-1][1] + (255,)


def main():
    target = Path(__file__).resolve().parent / "qml/chrome/floor-light.ico"
    image = Image.new("RGBA", (WIDTH, HEIGHT))
    for x in range(WIDTH):
        colour = colour_at(x / (WIDTH - 1))
        for y in range(HEIGHT):
            image.putpixel((x, y), colour)
    image.save(target, sizes=[image.size], bitmap_format="bmp")
    print(target.name)


if __name__ == "__main__":
    main()
