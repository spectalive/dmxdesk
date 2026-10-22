"""Check the visible Tempo Show, BPM, and BPM Base axes in a Directo capture."""

import argparse

from PIL import Image


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("capture")
    args = parser.parse_args()
    image = Image.open(args.capture).convert("RGB")
    if image.size != (1024, 600):
        parser.error("expected a 1024x600 Directo capture")

    bounds = []
    for name, box in (
        ("Tempo Show", (545, 62, 640, 85)),
        ("BPM", (545, 82, 650, 124)),
        ("BPM Base", (545, 125, 650, 142)),
    ):
        pixels = []
        for y in range(box[1], box[3]):
            for x in range(box[0], box[2]):
                red, green, blue = image.getpixel((x, y))
                if min(red, green, blue) > 125 and max(red, green, blue) - min(red, green, blue) < 35:
                    pixels.append(x)
        if not pixels:
            parser.error(f"no ink found for {name}")
        left, right = min(pixels), max(pixels)
        bounds.append((left, right))
        print(f"{name}: {left}..{right} px")

    bpm_offset = abs(bounds[0][0] - bounds[1][0])
    base_offset = abs(bounds[0][0] - bounds[2][0])
    print(f"BPM left offset: {bpm_offset} px; BPM Base left offset: {base_offset} px")
    return 0 if bpm_offset <= 1 and base_offset <= 1 else 1


if __name__ == "__main__":
    raise SystemExit(main())
