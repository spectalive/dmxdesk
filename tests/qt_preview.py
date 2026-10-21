"""Open the real Qt desk on a Mac against a harmless mock QLC+ master.

Pass a backend command that can reach this host, such as an SSH command for
the tablet's ARM backend. No show commands reach the live master.
"""

import argparse
import queue
import shlex
import subprocess
import tempfile
import threading
from pathlib import Path

from qt_mock_handler import QtMockHandler
from qt_mock_server import QtMockServer


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--connect-host", required=True)
    parser.add_argument("--backend-command", required=True)
    parser.add_argument("--capture")
    parser.add_argument("--font")
    parser.add_argument("--tap")
    parser.add_argument("--agent-stdio", action="store_true")
    args = parser.parse_args()
    repo = Path(__file__).resolve().parents[1]
    fixture = Path(__file__).parent / "dmx-desk/fixtures/vc-vibra.json"
    with QtMockServer(("0.0.0.0", 0), QtMockHandler) as server:
        server.fixture = fixture.read_bytes()
        server.frames = queue.Queue()
        threading.Thread(target=server.serve_forever, daemon=True).start()
        with tempfile.TemporaryDirectory(prefix="qt-desk-preview-") as temp:
            wrapper = Path(temp) / "backend"
            command = shlex.split(args.backend_command)
            command += ["--host", args.connect_host, "--port", str(server.server_address[1])]
            wrapper.write_text(
                "#!/bin/sh\nexec " + " ".join(shlex.quote(part) for part in command)
                + ' "$@"\n'
            )
            wrapper.chmod(0o700)
            qt = repo / "output/qt-host-build/qt-desk"
            launch = [
                str(qt), "--backend", str(wrapper),
                "--map", "/usr/share/dmxdesk/vibra.desk.json",
                "--qml", str(repo / "qt/qml/Main.qml"),
            ]
            if args.capture:
                launch += ["--capture", args.capture]
            if args.font:
                launch += ["--font", args.font]
            if args.tap:
                launch += ["--tap", args.tap]
            if args.agent_stdio:
                launch += ["--agent-stdio"]
            try:
                raise SystemExit(subprocess.call(launch))
            finally:
                server.shutdown()


if __name__ == "__main__":
    main()
