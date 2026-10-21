"""Render a white Qt icon from SVG while preserving only its alpha shape."""

import argparse
import io
import subprocess
from pathlib import Path

from PIL import Image


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    png = subprocess.run(
        ["rsvg-convert", "-w", "96", "-h", "96", str(args.source)],
        check=True,
        capture_output=True,
    ).stdout
    rendered = Image.open(io.BytesIO(png)).convert("RGBA")
    icon = Image.new("RGBA", rendered.size, (245, 247, 250, 0))
    icon.putalpha(rendered.getchannel("A"))
    icon.save(args.output)


if __name__ == "__main__":
    main()
