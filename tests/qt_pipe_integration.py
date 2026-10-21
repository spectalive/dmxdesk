"""Exercise a real desk backend against a local mock master.

The command and reachable host are supplied by the caller so this also works
against the ARM binary through SSH without storing device access in this repo.
"""

import argparse
import json
import queue
import select
import shlex
import subprocess
import threading
import time
from pathlib import Path

from qt_mock_handler import QtMockHandler
from qt_mock_server import QtMockServer


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--connect-host", required=True)
    parser.add_argument("--backend-command", required=True)
    args = parser.parse_args()
    fixture = Path(__file__).parent / "dmx-desk/fixtures/vc-vibra.json"
    with QtMockServer(("0.0.0.0", 0), QtMockHandler) as server:
        server.fixture = fixture.read_bytes()
        server.frames = queue.Queue()
        threading.Thread(target=server.serve_forever, daemon=True).start()
        command = shlex.split(args.backend_command) + [
            "--qt-pipe", "--host", args.connect_host,
            "--port", str(server.server_address[1]),
        ]
        child = subprocess.Popen(
            command, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL, text=True, bufsize=1,
        )
        try:
            deadline = time.monotonic() + 8
            view = None
            while time.monotonic() < deadline:
                if not select.select([child.stdout], [], [], 0.2)[0]:
                    continue
                line = child.stdout.readline()
                if line.startswith("@STATE "):
                    candidate = json.loads(line[7:])
                    if candidate["link"] == 3:
                        view = candidate
                        break
            assert view, "desk did not link to the mock master"
            for x, expected_page in ((230, 1), (80, 0)):
                at = int(time.monotonic() * 1000)
                child.stdin.write(f"D 0 {x} 24 {at}\nU 0 {x} 24 {at + 30}\n")
                child.stdin.flush()
                deadline = time.monotonic() + 2
                changed = False
                while time.monotonic() < deadline:
                    if not select.select([child.stdout], [], [], 0.2)[0]:
                        continue
                    line = child.stdout.readline()
                    if line.startswith("@STATE ") and json.loads(line[7:])["page"] == expected_page:
                        changed = True
                        break
                assert changed, f"tab did not select page {expected_page}"
            color_mode = next(control for control in view["controls"]
                              if control["label"] == "GOLPES DE COLOR")
            x = color_mode["x"] + color_mode["w"] // 2
            y = color_mode["y"] + color_mode["h"] // 2
            at = int(time.monotonic() * 1000)
            child.stdin.write(f"D 0 {x} {y} {at}\nU 0 {x} {y} {at + 30}\n")
            child.stdin.flush()
            deadline = time.monotonic() + 2
            color_shot = False
            while time.monotonic() < deadline:
                if not select.select([child.stdout], [], [], 0.2)[0]:
                    continue
                line = child.stdout.readline()
                if line.startswith("@STATE ") and json.loads(line[7:])["colorOneShot"]:
                    color_shot = True
                    break
            assert color_shot, "colour-hit switch did not change the view"
            flash = next(control for control in view["controls"] if control["label"] == "FLASH")
            x = flash["x"] + flash["w"] // 2
            y = flash["y"] + flash["h"] // 2
            child.stdin.write(f"D 0 {x} {y} {int(time.monotonic() * 1000)}\n")
            child.stdin.flush()
            received = []
            deadline = time.monotonic() + 3
            while time.monotonic() < deadline:
                try:
                    received.append(server.frames.get(timeout=0.2))
                except queue.Empty:
                    pass
                if any(frame.endswith("|255") for frame in received):
                    break
            on = next((frame for frame in received if frame.endswith("|255")), None)
            assert on, f"flash press absent: {received}"
            stop = next(control for control in view["controls"] if control["label"] == "PARAR TODO")
            stop_x = stop["x"] + stop["w"] // 2
            stop_y = stop["y"] + stop["h"] // 2
            at = int(time.monotonic() * 1000)
            child.stdin.write(
                f"D 1 {stop_x} {stop_y} {at}\n"
                f"U 1 {stop_x} {stop_y} {at + 30}\n"
            )
            child.stdin.flush()
            show_map = json.loads(
                (Path(__file__).parents[1] / "show/vibra.desk.json").read_text()
            )
            stop_frame = f"{show_map['stopAll']['widget']}|255"
            deadline = time.monotonic() + 2
            while time.monotonic() < deadline:
                try:
                    received.append(server.frames.get(timeout=0.2))
                except queue.Empty:
                    pass
                if stop_frame in received:
                    break
            assert stop_frame in received, f"second finger did not reach stop-all: {received}"
            child.stdin.close()
            off = on.rsplit("|", 1)[0] + "|0"
            deadline = time.monotonic() + 4
            while time.monotonic() < deadline:
                try:
                    received.append(server.frames.get(timeout=0.2))
                except queue.Empty:
                    pass
                if off in received:
                    break
            child.wait(timeout=3)
            assert off in received, f"flash release absent after EOF: {received}"
            assert child.returncode == 0, f"backend exited with {child.returncode}"
            print("mock master: linked, tabs, colour-hit switch, two-finger stop-all and EOF release passed")
        finally:
            if child.poll() is None:
                child.kill()
                child.wait()
            server.shutdown()


if __name__ == "__main__":
    main()
