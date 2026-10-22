"""Verify the real Qt capture command rejects the observed mixed layout."""

import json
import os
from pathlib import Path
import subprocess
import tempfile
import unittest


class QtCaptureGateTest(unittest.TestCase):
    executable = Path(os.environ.get("DMXDESK_QT_EXECUTABLE",
                                    Path(__file__).resolve().parents[1] / "output/qt-host-build/qt-desk"))
    fixture = Path(__file__).resolve().parents[1] / "qt/tests/fixtures/direct-effects.json"

    def check_capture(self, broken):
        state = json.loads(self.fixture.read_text())
        if broken:
            for index in (0, 1, 3, 4):
                state["controls"][index].update(
                    x=540 if index % 3 == 0 else 659,
                    y=184 if index < 3 else 276, w=111, h=84)
        with tempfile.TemporaryDirectory(prefix="qt-capture-gate-") as directory:
            root = Path(directory)
            backend = root / "backend"
            backend.write_text('#!/bin/sh\ncat "$0.state"\ncat >/dev/null\n')
            backend.chmod(0o700)
            backend.with_suffix(".state").write_text("@STATE " + json.dumps(state) + "\n")
            capture = root / "capture.png"
            result = subprocess.run(
                [str(self.executable), "--backend", str(backend), "--qml",
                 str(self.fixture.parents[2] / "qml/Main.qml"), "--capture", str(capture)],
                env={**os.environ, "QT_QPA_PLATFORM": "offscreen", "QT_QUICK_BACKEND": "software"},
                capture_output=True, text=True, timeout=12)
            self.assertTrue(capture.is_file(), result.stderr)
            self.assertEqual(result.returncode, 1 if broken else 0, result.stdout + result.stderr)
            self.assertIn("@AGENT LAYOUT_ERROR overlap" if broken else "@AGENT VERIFY 0", result.stdout)

    def test_current_layout_capture_succeeds(self):
        self.check_capture(False)

    def test_mixed_layout_capture_fails(self):
        self.check_capture(True)


if __name__ == "__main__":
    unittest.main()
