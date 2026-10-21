# Qt Quick desk design

## Intent

Build a fully functional Qt Quick version of the TAQ-102 lighting desk so the
owner can compare it with the established C interface on the physical tablet.
The first Qt spike established that EGLFS and touch work; it was a visual mock
and did not control QLC+. This version must show and operate the real show.

## Scope and acceptance

- Reproduce the established desk's visual hierarchy, icon vocabulary, 18 rig
  colours, one-shot colour switch, exact mode and effect captions, ambient
  smoke intervals, tempo readout and one-BPM steps, master fader, stop-all,
  current-status card, connection state and settings surface.
- Cover DIRECTO, EFECTOS, RITMO, settings, lock and blank states. Pages and
  controls come from the validated desk map, not hard-coded widget IDs.
- A press reaches QLC+ through the existing desk session and models. Qt owns
  rendering and touch; the C backend owns show validation, actions, hold caps,
  owed releases, reconnection, speed echo and persistent device settings.
- A momentary action is released on touch-up, cancellation, Qt disconnect,
  lock or page change. The backend must not replay a queued toggle after a
  reconnect. The UI reports the master's state, not an assumed local state.
- The existing C desk remains the boot default while the Qt build is being
  compared. A failed Qt process returns the tablet to the established desk.
- Verify with a mock master and host tests before letting the Qt trial reach
  a live show. Physical smoke and white-burst output require controlled rig
  checks; software tests alone do not prove optical or pump behaviour.

## Architecture

The C desk gains an optional headless backend mode over standard input and
output pipes. Qt launches it as its child process. The backend continues to
run the existing input dispatch and QLC+ runtime, accepting Qt touch events in
panel coordinates. It publishes a bounded JSON view snapshot containing
validated controls, placements, speed, status and settings, with no Wi-Fi
passwords or credentials. Pipe EOF cancels all gestures and drains owed
releases. The ordinary DRM desk path stays intact.

A separate Qt Quick executable uses those pipes through a small C++ bridge.
QML renders each page from snapshots and sends press, move, release and cancel
events with stable touch IDs. Icon artwork is derived from the existing vector
set without emoji. The layout is fixed at 1024x600 on this tablet; a desktop
preview can scale the same canvas for review. The C desk's captured frames are
the visual reference, not the prototype's approximate layout.

Buildroot carries Qt only in a separate trial image/configuration until the
owner has compared the complete version. The trial must never replace the
recoverable C image as the sole boot path.

## Verification

Host tests cover pipe framing, malformed input, multiple fingers, disconnect
during a held flash or fog command, reconnect, disabled controls, show mismatch
and snapshot parity against the C model. A Qt host preview checks every page
and state at 1024x600. The tablet run checks first paint, touch mapping, memory
and successful return to the C desk. Live QLC+ actions are checked only after
mock-master tests pass.
