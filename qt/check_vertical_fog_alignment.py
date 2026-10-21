"""Check the rendered ink centers in a 1024x600 Directo capture."""

import argparse

from PIL import Image


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("capture")
    args = parser.parse_args()
    image = Image.open(args.capture).convert("RGB")
    if image.size != (1024, 600):
        parser.error("expected a 1024x600 Directo capture")

    centers = []
    for name, box in (
        ("left icon", (615, 371, 647, 401)),
        ("label", (659, 374, 769, 398)),
        ("right icon", (781, 371, 813, 401)),
    ):
        pixels = []
        for y in range(box[1], box[3]):
            for x in range(box[0], box[2]):
                red, green, blue = image.getpixel((x, y))
                if min(red, green, blue) > 180 and max(red, green, blue) - min(red, green, blue) < 20:
                    pixels.append(y)
        if not pixels:
            parser.error(f"no near-white ink found for {name}")
        center = sum(pixels) / len(pixels)
        centers.append(center)
        print(f"{name}: {center:.2f} px")

    offset = abs((centers[0] + centers[2]) / 2 - centers[1])
    mirror_offset = abs(centers[0] - centers[2])
    print(f"optical offset: {offset:.2f} px; icon pair: {mirror_offset:.2f} px")
    return 0 if offset <= 1.0 and mirror_offset <= 0.5 else 1


if __name__ == "__main__":
    raise SystemExit(main())
