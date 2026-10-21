"""Render the desk's own icon masks for Qt Quick."""

import re
from pathlib import Path

from PIL import Image


def main():
    root = Path(__file__).resolve().parents[1]
    source = (root / "src/icon_data.h").read_text()
    output = root / "qt/qml/icons"
    output.mkdir(parents=True, exist_ok=True)
    for name, body in re.findall(r"\[(ICON_[A-Z_0-9]+)\] = \{([^}]*)\}", source):
        values = bytes(int(value) for value in re.findall(r"\b\d+\b", body))
        if len(values) == 576:
            size = 24
        elif len(values) == 1600:
            size = 40
        else:
            continue
        icon = Image.new("RGBA", (size, size), (246, 249, 254, 0))
        icon.putalpha(Image.frombytes("L", (size, size), values))
        icon.resize((size * 2, size * 2), Image.Resampling.LANCZOS).save(
            output / f"{name[5:].lower()}.png"
        )


if __name__ == "__main__":
    main()
