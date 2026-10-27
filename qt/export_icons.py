"""Render the desk's own icon masks for Qt Quick.

The 24 px set is drawn again from tools/icons.py's vectors at 48 px, so the
Qt desk shows crisp marks instead of an upscaled 24 px bitmap.
"""

import importlib.util
import re
from pathlib import Path

from PIL import Image


def load_vectors(root):
    spec = importlib.util.spec_from_file_location("desk_icons", root / "tools/icons.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def main():
    root = Path(__file__).resolve().parents[1]
    source = (root / "core/icon_data.h").read_text()
    output = root / "qt/qml/icons"
    output.mkdir(parents=True, exist_ok=True)
    vectors = load_vectors(root)
    for name, body in re.findall(r"\[(ICON_[A-Z_0-9]+)\] = \{([^}]*)\}", source):
        values = bytes(int(value) for value in re.findall(r"\b\d+\b", body))
        if len(values) == 576:
            size = 24
        elif len(values) == 1600:
            size = 40
        else:
            continue
        icon = Image.new("RGBA", (size * 2, size * 2), (246, 249, 254, 0))
        if size == 24:
            icon.putalpha(vectors.baked(vectors.DRAW_OF[name], size * 2))
        else:
            mask = Image.frombytes("L", (size, size), values)
            icon.putalpha(mask.resize((size * 2, size * 2), Image.Resampling.LANCZOS))
        icon.save(output / f"{name[5:].lower()}.png")


if __name__ == "__main__":
    main()
