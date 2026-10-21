# Qt Quick desk trial

This is a second visual surface for the existing desk engine. The C program
runs as a headless child and keeps ownership of QLC+ validation, gestures,
holds, tempo, settings and connection recovery. Qt receives bounded state
snapshots and returns panel coordinates. The installed C desk remains the
tablet's default until the physical trial has been compared and approved.
The 1024x600 EGLFS capture is in
`docs/evidence/qt-desk-panel-2026-10-21.png`.
The later Mac review captures in `docs/evidence/qt-desk-text-2026-10-21.png`
and `docs/evidence/qt-desk-color-hits-2026-10-21.png` show the corrected
mode label, aligned status fields, color-hit marks and named effect banks.
`docs/evidence/qt-desk-spacing-2026-10-21.png` shows the subsequent centered
icon, label and hold-prompt groups in Directo.

## Host checks

Build with Qt 6.9 or newer:

```sh
cmake -S qt -B output/qt-host-build -DCMAKE_PREFIX_PATH=/path/to/qt
cmake --build output/qt-host-build
ctest --test-dir output/qt-host-build --output-on-failure
tools/test-dmx-desk-host.sh
```

`qt/export_icons.py` regenerates the 48 px controls and 80 px trajectory
assets from `src/icon_data.h`.
It needs Pillow only when the artwork is regenerated; Qt needs no Python at
runtime. The app loads `qt/qml/Main.qml` from disk, so QML can be iterated
without relinking the executable.

`tests/qt_pipe_integration.py` starts a local mock QLC+ master and verifies a
real backend's Flash activation and its release after pipe EOF. Supply a
backend command and an address that backend can reach; the script stores no
device access details. Run this check before any live QLC+ trial.

For fast visual work on a Mac, `tests/qt_preview.py` starts the same mock
master and opens the host Qt window with a remote ARM backend. Pass the
backend command and a host address it can reach, as with the integration
test. `--capture output/preview.png` saves a frame and exits; omit it to use
the window. `--font /path/to/Inter-Regular.ttf` loads the tablet's font for
closer visual comparison. The mock master never forwards commands to QLC+.
With `--tap 230,24 --capture output/effects.png`, the app selects a tab before
capturing it, which makes repeatable screenshots of other pages quick.
For an interactive agent session, pass `--agent-stdio` and send one command
per line to its stdin: `TAP x y`, `CAPTURE /absolute/path.png`, or `QUIT`.
`TOUCH D|M|U|C slot x y` exposes held controls, drags and multiple touch slots.
`AUDIT` reports coordinates of visible Qt Text items that overflow or elide,
without printing their contents. Review screenshots separately for spacing and
alignment; the text audit does not measure either.
Responses start with `@AGENT`. This control channel is opt-in and is never
enabled by the tablet launcher.

## Tablet trial

Build Qt Base with EGLFS, DRM and PNG, plus Qt Declarative Quick, in an
isolated Buildroot output. Cross-build `qt/` with that output's `qt-cmake`;
copy the executable, `qt/qml/`, backend, and matching Qt runtime to a trial
directory on `/data`. `qt-desk` accepts `--backend`, `--map`, `--qml` and
`--power`; its defaults point to `/data/qt-full` and the installed desk map.
Resolve the power-key input by device name before passing `--power`, because
event numbers vary between boots. The Qt EGLFS touch plugin reads the touch
device directly.

The tablet repository's `tools/run-qt-desk-trial.sh` handles the temporary
supervisor handoff, EGLFS environment, dynamic power-key lookup and return to
the C desk when Qt exits or reaches its thirty-minute cap. Confirm that the
normal desk links again before leaving the tablet. Do not install the trial
as the boot default from this directory.
