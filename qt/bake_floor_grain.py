"""Bake the Stage Console floor grain into a tileable ICO.

The concept's floor is a dark leather with a grain of about four levels
either way. Noise blurred on a wrapped canvas tiles without seams; it is
stored as white or black with a small alpha so the backdrop's colour shows
through, as BMP-encoded ICO because the tablet's Qt reads no PNG.
"""
import random
from pathlib import Path

from PIL import Image, ImageFilter

SIZE = 256
SEED = 2026
BLUR = 1.2
AMPLITUDE = 8


def grain():
    rng = random.Random(SEED)
    noise = Image.new("L", (SIZE, SIZE))
    noise.putdata([rng.randrange(256) for _ in range(SIZE * SIZE)])
    wrapped = Image.new("L", (SIZE * 3, SIZE * 3))
    for dx in range(3):
        for dy in range(3):
            wrapped.paste(noise, (dx * SIZE, dy * SIZE))
    blurred = wrapped.filter(ImageFilter.GaussianBlur(BLUR)).crop((SIZE, SIZE, 2 * SIZE, 2 * SIZE))
    return blurred.point(lambda v: 128 + (v - 128) * 6)


def tile(levels):
    light = levels.point(lambda v: max(0, min(AMPLITUDE, (v - 128) // 8)))
    dark = levels.point(lambda v: max(0, min(AMPLITUDE, (128 - v) // 8)))
    image = Image.new("RGBA", (SIZE, SIZE))
    white = Image.new("RGBA", (SIZE, SIZE), (255, 255, 255, 0))
    white.putalpha(light)
    black = Image.new("RGBA", (SIZE, SIZE), (0, 0, 0, 0))
    black.putalpha(dark)
    return Image.alpha_composite(Image.alpha_composite(image, black), white)


def main():
    target = Path(__file__).resolve().parent / "qml/chrome/floor-grain.ico"
    image = tile(grain())
    image.save(target, sizes=[image.size], bitmap_format="bmp")
    print(target.name)


if __name__ == "__main__":
    main()
