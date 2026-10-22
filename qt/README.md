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
`docs/evidence/qt-desk-style-direct-2026-10-21.png` is a physical tablet
capture after restoring the room, Stop All and spacing treatment; the Mac
preview of the restored effect palette is in
`docs/evidence/qt-desk-style-colors-2026-10-21.png`.
The later Directo hold revision is captured on the tablet in
`docs/evidence/qt-desk-holds-2026-10-21.png`.
`docs/evidence/qt-desk-loop-2026-10-21.png` captures the restored one-line
effects heading and large BPM, plus compact ambient haze intervals marked
with a cloud and repeat symbol on the physical tablet.
`docs/evidence/qt-desk-mac-final-2026-10-21.png` captures the subsequent Mac
alignment pass: centered tempo captions, the complete Wi-Fi mark, a clearer
repeat arrow, and more space below the effects heading.
`docs/evidence/qt-desk-mac-tempo-cloud-2026-10-21.png` captures the corrected
Directo tempo grouping and the single cloud/repeat icon.
`docs/evidence/qt-desk-alignment-2026-10-21.png` captures the Mac pass with
the full Humo vertical label, paired smoke icons, centered OFF, and the
monochrome ambient-loop mark. The editable icon sources are in `qt/assets/`;
regenerate their monochrome PNGs with:

```sh
python3 qt/render_monochrome_icon.py qt/assets/cloud_loop.svg qt/qml/icons/cloud_loop.png
python3 qt/render_monochrome_icon.py qt/assets/vertical_fog.svg qt/qml/icons/vertical_fog.png
```

`docs/evidence/qt-desk-optical-alignment-2026-10-21.png` captures the smoke
icons after their rendered ink centers were aligned with the label. Run
`python3 qt/check_vertical_fog_alignment.py /path/to/directo-capture.png`
against a fresh 1024x600 Mac or tablet capture: it fails when the icons'
visible centers differ from the label by more than one pixel. The preceding
capture fails by 1.64 px; the corrected one passes at 0.36 px.
`docs/evidence/qt-desk-selected-color-2026-10-21.png` compares selected and
unselected red swatches in an isolated Qt render. Selection uses the existing
outline without a check mark over the color.
`docs/evidence/qt-effects-padding-2026-10-22.png` is an isolated Qt render
using the revised C layout coordinates: all seven effects have 12 px between
their edges and the panel, with the wide fog pad clear of the bottom edge.
The buttons' touch rectangles use those same coordinates. The tablet backend
still needs a new build before it will supply the revised layout to Qt.
`docs/evidence/qt-tempo-alignment-2026-10-22.png` records the earlier centered
BPM Base treatment. Run the pixel check with
`python3 qt/check_tempo_alignment.py /path/to/directo-capture.png`; it checks
the visible left edges of Tempo Show, BPM and BPM Base in a fresh 1024x600
capture. The integrated render has zero offset for both labels.
`docs/evidence/qt-desk-mac-integrated-2026-10-22.png` shows the full Mac desk
with three aligned effects rows and the wider BPM numerals in one window.
All three tempo lines share one visible left edge. The installed tablet
backend still reports the earlier effect rectangles, so this preview
uses `tests/qt_layout_preview_bridge.py` to translate those seven rectangles
and their touches against the mock master. The bridge is only for preview;
the tablet needs its backend rebuilt to use the new layout directly.

## Host checks

Build with Qt 6.9 or newer:

```sh
cmake -S qt -B output/qt-host-build -DCMAKE_PREFIX_PATH=/path/to/qt
cmake --build output/qt-host-build
ctest --test-dir output/qt-host-build --output-on-failure
tools/test-dmx-desk-host.sh
python3 tests/qt_layout_preview_bridge_test.py
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
`AUDIT` checks visible Qt Text items for overflow or elision and checks tagged
items for parent centering, shared left or center axes, and mirrored icons.
It reports `@AGENT AUDIT 0` and `@AGENT ALIGNMENT 0` on a clean page, without
printing text contents. The alignment test deliberately displaces tempo, OFF,
and the paired smoke icons to verify that each error is detected. Review a
fresh screenshot for optical alignment and spacing, which geometry alone
cannot measure.
To preview a newer C effects layout against an older installed backend, wrap
the backend command with `tests/qt_layout_preview_bridge.py`. The wrapper
translates only the seven known effect rectangles and their touch coordinates;
other controls pass through. It is a temporary visual aid until the tablet
backend is rebuilt.
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
