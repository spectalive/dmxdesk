"""Bake the Stage Console decoration into 9-slice ICO images.

Qt renders every look in chrome-baker/Bake.qml with the drawing components
beside it. Controls get two passes: the halo and recess outside the control,
whose interior is cut away so the live fill colour stays visible, and the
face (sheen, inner glow, rim) laid over it. Panels are baked whole. The
tablet's Qt reads ICO but not PNG, so the results are BMP-encoded ICO files
in qml/chrome/. Needs the host `qml` tool and Pillow; run after changing a look.
"""
import os
import re
import subprocess
import tempfile
from pathlib import Path

from PIL import Image, ImageDraw

MARGIN = 16
SUPERSAMPLE = 4


def bake_passes(baker, out):
    env = dict(os.environ, QT_QPA_PLATFORM="offscreen")
    subprocess.run(["qml", str(baker), "--", str(out)], check=True, env=env, timeout=120)


def interior_mask(size, inner, radius):
    big = Image.new("L", (size * SUPERSAMPLE, size * SUPERSAMPLE), 255)
    box = [MARGIN * SUPERSAMPLE, MARGIN * SUPERSAMPLE,
           (MARGIN + inner) * SUPERSAMPLE - 1, (MARGIN + inner) * SUPERSAMPLE - 1]
    ImageDraw.Draw(big).rounded_rectangle(box, radius * SUPERSAMPLE, fill=0)
    return big.resize((size, size), Image.LANCZOS)


def radius_of(baker_source, name):
    match = re.search(r'name: "%s", r: (\d+)' % re.escape(name), baker_source)
    return int(match.group(1))


def main():
    here = Path(__file__).resolve().parent
    baker = here / "chrome-baker/Bake.qml"
    target = here / "qml/chrome"
    target.mkdir(exist_ok=True)
    source = baker.read_text()
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        bake_passes(baker, tmp)
        for panel in sorted(tmp.glob("*.panel.png")):
            name = panel.name.split(".")[0]
            image = Image.open(panel).convert("RGBA")
            image.save(target / f"{name}.ico", sizes=[image.size], bitmap_format="bmp")
        for halo_path in sorted(tmp.glob("*.halo.png")):
            name = halo_path.name.split(".")[0]
            halo = Image.open(halo_path).convert("RGBA")
            face = Image.open(tmp / f"{name}.face.png").convert("RGBA")
            inner = halo.width - 2 * MARGIN
            alpha = Image.composite(halo.getchannel("A"), Image.new("L", halo.size, 0),
                                    interior_mask(halo.width, inner, radius_of(source, name)))
            halo.putalpha(alpha)
            image = Image.alpha_composite(halo, face)
            image.save(target / f"{name}.ico", sizes=[image.size], bitmap_format="bmp")
    print("\n".join(sorted(p.name for p in target.glob("*.ico"))))


if __name__ == "__main__":
    main()
