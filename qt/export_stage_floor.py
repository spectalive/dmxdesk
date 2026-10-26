"""Write the Stage Console floor as one window-sized BMP.

ChatGPT exported the concept's floor without any controls
(docs/design/futuristic-2026-10-24/elements/floor.png): the leather grain,
the vignette and the stage light falling on it. It is scaled to the window
and stored as BMP, which the tablet's Qt reads without a plugin; the floor
is opaque, so it needs no alpha. It is dimmed to sit as dark between the
panels as the concept does.
"""
from pathlib import Path

from PIL import Image, ImageEnhance

WINDOW = (1024, 600)
BRIGHTNESS = 0.8


def main():
    here = Path(__file__).resolve().parent
    floor = Image.open(here.parent / "docs/design/futuristic-2026-10-24/elements/floor.png").convert("RGB")
    target = here / "qml/chrome/floor.bmp"
    floor = floor.resize(WINDOW, Image.LANCZOS)
    ImageEnhance.Brightness(floor).enhance(BRIGHTNESS).save(target)
    print(target.name)


if __name__ == "__main__":
    main()
