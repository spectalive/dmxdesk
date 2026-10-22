"""Preview the current live-effect layout with a legacy tablet backend.

The bridge runs only behind qt_preview.py's mock master. It rewrites the
seven effect rectangles in snapshots and maps their touch coordinates back
to the installed backend until that backend is rebuilt.
"""

import argparse
import json
import shlex
import subprocess
import sys
import threading


class QtLayoutPreviewBridge:
    rectangles = {
        "FLASH": ((540, 184, 110, 84), (552, 184, 103, 78)),
        "FLASH LENTO": ((659, 184, 110, 84), (663, 184, 103, 78)),
        "FLASH COLOR": ((778, 184, 110, 84), (774, 184, 102, 78)),
        "STROBO": ((540, 276, 110, 84), (552, 270, 103, 78)),
        "STROBO SUAVE": ((659, 276, 110, 84), (663, 270, 103, 78)),
        "HUMO YA": ((778, 276, 110, 84), (774, 270, 102, 78)),
        "HUMO VERT": ((540, 368, 348, 56), (552, 356, 324, 56)),
    }

    def __init__(self):
        self.legacy_layout = False

    def state_line(self, line):
        if not line.startswith("@STATE "):
            return line
        try:
            state = json.loads(line[7:])
            for control in state.get("controls", []):
                pair = self.rectangles.get(control.get("label"))
                if pair and tuple(control.get(key) for key in ("x", "y", "w", "h")) == pair[0]:
                    control.update(zip(("x", "y", "w", "h"), pair[1]))
                    self.legacy_layout = True
            return "@STATE " + json.dumps(state, separators=(",", ":")) + "\n"
        except (TypeError, ValueError):
            return line

    def touch_line(self, line):
        if not self.legacy_layout:
            return line
        parts = line.split()
        if len(parts) != 5 or parts[0] not in ("D", "M", "U", "C"):
            return line
        try:
            x, y = int(parts[2]), int(parts[3])
        except ValueError:
            return line
        if not (540 <= x < 888 and 160 <= y < 424):
            return line
        old_point = (-1, -1)
        for old, new in self.rectangles.values():
            ox, oy, ow, oh = old
            nx, ny, nw, nh = new
            if nx <= x < nx + nw and ny <= y < ny + nh:
                old_point = (ox + (x - nx) * ow // nw,
                             oy + (y - ny) * oh // nh)
                break
        parts[2], parts[3] = map(str, old_point)
        return " ".join(parts) + "\n"

    def forward_input(self, child):
        for line in sys.stdin:
            try:
                child.stdin.write(self.touch_line(line))
                child.stdin.flush()
            except BrokenPipeError:
                break
        child.stdin.close()

    def run(self):
        parser = argparse.ArgumentParser()
        parser.add_argument("--backend-command", required=True)
        args, rest = parser.parse_known_args()
        child = subprocess.Popen(shlex.split(args.backend_command) + rest,
                                 stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                 stderr=sys.stderr, text=True, bufsize=1)

        threading.Thread(target=self.forward_input, args=(child,), daemon=True).start()
        for line in child.stdout:
            sys.stdout.write(self.state_line(line))
            sys.stdout.flush()
        return child.wait()


if __name__ == "__main__":
    raise SystemExit(QtLayoutPreviewBridge().run())
