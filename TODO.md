# TODO

> Consolidated from the accessible agent conversation history of this project.
> Last reviewed: 2026-10-12. History coverage: Partial. Not re-audited during
> the 2026-10-12 format migration.
>
> States: `[ ]` pending · `[~]` partial or unverified · `[!]` blocked · `[x]`
> verified complete · `[-]` obsolete or superseded. Closed work moves to
> `TODO_LOG.md`.

The desk's open work from before the split (2026-09-26) is still in
spectalive/taq102's `TODO.md` under "DMX desk", because almost all of it needs
the tablet. Items here are about this repository on its own.

## Tests

- [ ] **Make the Qt pipe integration map requirement explicit.** On 2026-10-24, the installed backend did not link when the command omitted its map; adding `--map /usr/share/dmxdesk/vibra.desk.json` passed tabs, colour-hit mode, two-finger Stop All and EOF Flash release. Next: expose a map argument in `tests/qt_pipe_integration.py` and preserve redacted backend diagnostics when linking fails.

## Packaging

- [ ] **Update the taq102 package for the core/platform split before the next version bump.** On 2026-10-27 `src/` became `core/` and `platform/linux-fb/`, and the source list moved to `dmxdesk.sources` at the repository root with paths. spectalive/taq102's `br2-external/package/dmxdesk/dmxdesk.mk` still builds `$(@D)/src/` with `-I$(@D)/src`, which is right for the pinned release but fails on any tag after the split. Next: when bumping, change it to `-I$(@D)/core -I$(@D)/platform/linux-fb` and `$(addprefix $(@D)/,$(shell cat $(@D)/dmxdesk.sources))`. The Buildroot cross-compiler already links the split tree, and its stripped binary is byte-identical to the pre-split one.

## Core

- [ ] **Give the desk a surface model of its own instead of QLC+'s virtual console document.** On 2026-10-28 the console moved behind `struct desk_console` with desk commands and events, and QLC+ became the first adapter (`core/qlc_console.c`). The model is still built from `struct vc_doc` (`showmap_build`, `desk_rebuild_model`, `desk_speed_validate`), so a second adapter (Onyx, MagicQ, OSC, MIDI) would have to fake QLC+'s widget tree. Next: list what the model actually reads from the snapshot (widget ids, types, function ids, speed dials, StopAll) and define the smallest desk-side surface description that covers it; then make QLC+'s adapter translate `/vc.json` into it.

## Design

- [~] **Make the Stage Console theme fluent on the tablet.** Earlier runs showed the backend unlinked, so every tile was drawn disabled and the connection dot animated without pause; measure linked: start the mock master on the Mac (`tests/qt_mock_server.py` with the `vc-vibra.json` fixture on a fixed port) and run `/data/qt-full/taps/drive-mock.sh` with `BACKEND=/data/qt-full/backend-mock` (adds `--host <mac> --port <port>`). Linked on 2026-10-26: flat theme 18-22 ms p50 per frame, Stage Console 58 ms with ~145 draw batches. Grouping every tile's ring, fill and baked look into `TileGlassLayers` (so they batch) and baking live tiles' glass into `fill-*` looks brought it to 36 ms and 59 batches (flat: ~50). Frame intervals land on 60 Hz vsync multiples (18, 36, 51 ms), so the GPU is now just over one refresh: removing the backdrop gave 24 ms, and any single full-window layer costs ~12 ms whether a gradient Rectangle or an image. Runs vary by about 5 ms; compare variants over repeated runs. Tried without gain (36 ms, two runs each): caching the floor and panels in one `layer.enabled` texture, and opaque occluder rectangles inside panels and tiles (the GPU did not skip what they covered). Next: get under one refresh without losing the floor, e.g. draw the floor only where panels leave it visible, and confirm with repeated runs whether removing the floor really reaches one refresh. Later on 2026-10-26 the floor became one window-sized BMP (the ChatGPT floor plate) instead of two images and the live tiles took looks sliced from the ChatGPT tile sheet: linked runs gave 37 and then 36 ms medians, level with before, and no image warnings. On 2026-10-27 the cause of the ~12 ms per full-window layer was found: the Mali-400 runs at a fixed 148.5 MHz (`aclk_gpu` = gpll 594 MHz / 4, no devfreq), because taq102 v67 removed the GPU OPP table after a hang at 480 MHz (taq102 `TODO.md`, RK816 regulator item). The vendor table (`kernel/rk3126-taq102-hybrid.dts`, phandle 0x19) gives 300 MHz at 1.05 V, and the logic rails read 1.1 V, so a fixed `assigned-clock-rates` of 297 MHz (gpll_div2) on `&gpu` would stay in spec without regulator control and should roughly halve GPU time. It needs a taq102 boot image flashed to `boot` and `recovery`. Blocked: the owner decides on the firmware later (2026-10-27); do not flash or touch the venue machines until then. The C desk draws through dumb buffers, not the GPU, so a GPU hang would only stop the Qt trial.
- [~] **Finish the Qt Quick desk trial on the physical tablet.**
  The Qt surface renders real C desk snapshots on the 1024x600 EGLFS panel;
  its backend has linked to QLC+, and the mock-master test covered tabs,
  colour-hit mode, two-finger stop-all and held-Flash release. The trial
  launcher returned to the linked C desk after capture. Real tablet touches
  switched show controls. The Mac preview now supports agent taps, holds and
  captures against a mock master. A Mac audit of visible Qt Text items found
  zero overflows across Directo, Ritmo, Ajustes, keyboard and every Efectos
  family and bank after the text corrections. A later bounded tablet capture
  restored the C process, but its configured master then refused connections;
  the owner identified another QLC+ instance as the cause. Both the C desk
  and Mac Qt trial later reconnected to the intended show. A visual spacing
  pass centered hold prompts, mode labels and trajectory labels by their
  actual content height; the updated QML rendered on the tablet offscreen.
  The owner's comparison found that the Qt trial had lost the effect colour
  squares, the red Stop All face, the active room accent and comfortable gaps
  between icons and labels. These faces were restored, reviewed in the Mac
  preview, and captured on the physical tablet; the C desk resumed and linked.
  The owner preferred the earlier compact hold pads, so Directo again places
  each mark beside its name with a quiet MANTENER footer, and gives fog holds
  their teal field. A second physical capture verified the revised layout.
  A subsequent tablet capture restored the one-line effects heading and large
  BPM, and showed cloud/repeat icons with short ambient haze intervals; the C
  desk resumed and linked after capture. The owner then asked for a Mac-only
  visual pass: the Wi-Fi mark now shows every layer and the effects heading
  has more room. The owner clarified that Directo's number must align with
  its label; the Mac preview now puts the number and BPM base on one axis
  below the left heading, and timed haze uses a single custom cloud/repeat
  asset. The latest Mac pass removed the transient pending dot, centered OFF,
  made the ambient mark monochrome, and placed the full Humo vertical label
  between paired smoke icons. A new runtime alignment audit and regression
  test check the relevant axes; the final Mac capture also confirmed their
  optical centers to the pixel. A follow-up optical-ink check exposed a
  1.64 px vertical imbalance in those icons, now reduced to 0.36 px; the
  selected swatch also uses its outline without a check mark. The latest Mac
  pass puts 12 px of padding around the effects in the shared C layout and
  aligns the rendered Tempo Show/BPM left edges. A later Mac capture exposed
  an incomplete preview translation that left Humo Ya overlapping Humo
  vertical. The bridge now translates all seven rectangles, tests their row
  gaps, and the full Mac screenshot shows aligned effects. BPM numerals are
  wider, and Tempo Show and BPM have the same visible left edge. The redundant
  BPM Base captions were removed from Directo and Ritmo; Directo's title and
  number now use the freed space for a 56 px numeral display, verified in the
  Mac capture with zero visible left-edge offset and a clean runtime audit.
  The BPM now loads its bundled Inter Display Bold face explicitly; after
  correcting its 2 px leading-edge shift, the fresh optical capture confirms
  zero offset again.
  The Mac status bar now fills the battery by charge and uses an 18x14 px
  Wi-Fi fan. Rendered-state tests cover empty, partial and full batteries,
  unknown charge, clamping and all four signal levels.
  Native QML motion now follows press, selection, hold countdown, status and
  page changes. The Mac pass verified persistent tiles, settled geometry,
  rapid release, reduced motion and immediate disconnected state; a 100-frame
  recording passed every capture audit. Tablet fluency still needs measuring.
  The tablet still runs the previous backend build. Next: resume the preserved
  Buildroot build, check the new layout on the physical panel, and verify
  remaining tablet gestures before considering any default-interface change.
- [~] **Confirm white and colour bursts on the physical rig.**
  On 2026-10-21, loopback Art-Net from QLC+ 5.2.2 showed AUTO colour on all
  four 7R wheels in 299/302 steady frames of the tablet's white burst; the
  console Flash button stayed white in 794/794. The tablet's Chaser has an
  eight-second server deadline but ordinary priority. The desk now presses a
  bounded, overriding Flash on the controller; the C host suite and loopback
  Art-Net test verify mapping, priority, expiry and disconnect. Next: measure
  the 7R wheels and smoke pumps at the physical FT232R output during Fiesta,
  then confirm release at the cap on the connected tablet.
- [ ] **Pulse at the show tempo (new, from the v3 mockups):** the RITMO cards
  and the SALA AHORA panel could blink with the BPM the desk already knows,
  so the room's beat is visible at a glance (`docs/design/v3/ritmo.png`,
  "nuevo" dot; rejected for v3 in `docs/design/v3/gaps.md`). The desk has no
  clock locked to QLC+'s tempo, so a free-running blink drifts from the show.
  Next: read whether QLC+'s web socket reports a beat or a tempo phase; if it
  does, paint a dot on its tick, otherwise drop it.

## Wi-Fi

- [ ] **Event socket after a supplicant restart:** `wpa_ctrl_open` ATTACHes
  the event socket once; a restarted wpa_supplicant forgets that, and
  `wpa_ctrl_event` only ever sees EAGAIN, so `CTRL-EVENT-SCAN-RESULTS` never
  queues the card's results and a join stops hearing events (found
  2026-10-19 reviewing v0.1.4, untested). Next: in `wpa_ctrl_test`, restart
  the fake and expect a SCAN's event; re-dial and re-ATTACH the event socket
  when a request had to redial.
