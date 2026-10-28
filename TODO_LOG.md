# TODO Log

> Searchable record of closed work. Active work lives in `TODO.md`.

<!-- tadu:log v1 since 2026-10-12 -->

## 2026

### 2026-10

- [x] 2026-10-28 — **Core:** **Give the desk a surface model of its own instead of QLC+'s virtual console document.** On 2026-10-28 the console moved behind `struct desk_console` with desk commands and events, and QLC+ became the first adapter (`core/qlc_console.c`). The model is still built from `struct vc_doc` (`showmap_build`, `desk_rebuild_model`, `desk_speed_validate`), so a second adapter (Onyx, MagicQ, OSC, MIDI) would have to fake QLC+'s widget tree. Next: list what the model actually reads from the snapshot (widget ids, types, function ids, speed dials, StopAll) and define the smallest desk-side surface description that covers it; then make QLC+'s adapter translate `/vc.json` into it.
  - Result: struct desk_surface (core/desk_surface.h) replaces QLC+'s vc_doc in validation, speed dials and the model; the QLC+ adapter translates /vc.json with qlc_surface_of
  - Evidence: 151112a. Host suite 54/54 (91 PASS), qlc_surface_test with a negative control, Qt ctest 7/7, ARM -Werror build clean, e2e --qt-pipe frames/states/logs identical to bdd589d in stall-free runs

- [x] 2026-10-27 — **Core:** **Take the desk's event loop out of `platform/linux-fb/dmxdesk.c`.** After the 2026-10-27 split and the move of the session actions into `core/` (`b0db25f`), `main` is still about 1180 lines that interleave the desk's own flow (snapshot rebuilds, gestures to actions, the setup surface, holds, repaint and damage) with the device calls (DRM present, evdev, power key, backlight, wpa_supplicant). Next: define a small platform interface (touch events in, a frame out, power and network status) and move the loop body into a core `desk_run` driven through it, so a second platform such as SDL reuses it unchanged; this is the first step of the multi-backend work.
  - Result: core/desk_run.c turns struct desk_app through struct desk_platform; linux-fb fills the table with linux_desk_* ops and main only opens devices and calls desk_run.
  - Evidence: d128041. Host suite 53/53 units PASS (core_without_platform, new desk_run_test with a scripted platform); Qt ctest 7/7; Buildroot ARM cross-build -Werror clean; --qt-pipe run against the mock master sends identical frames and states to the previous build.

- [x] 2026-10-26 — **Design:** **Choose a concrete visual reference before another Qt redesign.** On 2026-10-24, the owner rejected the pastel-panel pass and requested futuristic examples preserving the current distribution. Three reviewed concepts and the original screenshot are in `docs/design/futuristic-2026-10-24/`. Their control groups are retained, but spacing and idle-state illumination need refinement. The Mac app remains on the restored dark interface (`AUDIT`: `VERIFY 0`). Next: choose Precision Glass, Aerospace Instrument or Stage Console with the owner before another QML appearance pass.
  - Result: Stage Console (concept 3) applied to the Qt desk: ceramic gradients, violet/teal/red rim halos, dimmer idle controls; geometry unchanged.
  - Evidence: ctest 7/7; qt_preview --capture VERIFY 0 on Directo and Efectos; docs/evidence/qt-desk-stage-console-2026-10-26.png

- [x] 2026-10-24 — **Design:** Animate the Qt desk from its real state.
  - Result: Added native QML press faces, selection marks, settling icons, battery and Wi-Fi transitions, a moving tab indicator and page reveal. Kept tile instances across snapshots and tied hold countdowns to backend progress, with immediate disconnected state and support for reduced motion. Reopened the Mac preview; physical-tablet fluency remains in the Qt trial item.
  - Evidence: ctest --test-dir output/qt-host-build --output-on-failure: 7/7 passed in 15.93 s. The motion regression rejects the previous snapshot replacement. Mock-master pipe integration passed tabs, colour-hit mode, two-finger Stop All and EOF Flash release with an explicit map. All 100 recorded Mac frames passed VERIFY 0; see docs/evidence/qt-desk-motion-2026-10-24.mp4. Final optical checks: BPM left offset 0 px, vertical fog offset 0.53 px.

- [x] 2026-10-24 — **Design:** Render battery charge and balance the Qt Wi-Fi mark.
  - Result: The battery cell clips to the reported charge, and the Wi-Fi fan uses wider, shallower proportions alongside the existing status icons.
  - Evidence: ctest --test-dir output/qt-host-build --output-on-failure: 6/6 passed. The rendered-state test rejects the previous empty battery and square Wi-Fi footprint; charge levels -1/0/20/50/100/150 and signal levels 0..3 pass. The integrated Mac capture reports VERIFY 0; see docs/evidence/qt-desk-status-icons-2026-10-24.png.

- [x] 2026-10-22 — **Tests:** **Reject invalid Qt captures using rendered control geometry.**
  The text/axis audit accepted a mixed legacy/current effect layout with
  overlapping controls. Add rendered bounds, overlap and effect-grid checks
  to AUDIT and both capture paths; verify rejection using the real Main.qml
  and the observed 2026-10-22 layout before closing this gap.
  - Result: Added scene bounds, overlap, effect-grid and completeness checks to AUDIT and both capture modes. Invalid captures now fail, with regression cases loading the real Main.qml.
  - Evidence: ctest --test-dir output/qt-host-build --output-on-failure: 5/5 passed; the observed mixed layout and seven other invalid scenes are rejected, the capture process exits 1 on the observed failure, and integrated Mac Directo/Efectos/Ritmo audits report VERIFY 0. See docs/evidence/qt-layout-gate-2026-10-22.txt.

- [x] 2026-10-21 — **Connection status stays in the top bar.**
  - Result: A temporary master disconnect now shows a stable "Esperando conexión..." status without covering the controls; a different show still gets the mismatch card.
  - Evidence: `desk_render_test` checks the warning dot, unchanged content and identical connecting/syncing bar; the host suite and 19,764,864-probe geometry audit passed.

- [x] 2026-10-21 — **SHOW tablet control refinements.**
  - Result: The rig bank includes a white swatch, its colour hit respects GOLPES DE COLOR, Directo again offers BPM fine tuning, state and effect pads keep their icons and complete names, and the master and stop controls use less space.
  - Evidence: Host tests with address and undefined-behaviour sanitizers, the 19,764,864-probe geometry audit, Clang static analysis of changed controls, an ARM GCC 14.3.0 build, and the rendered SHOW screen.

- [x] 2026-10-21 — **SHOW tablet haze, colour hits and layout.**
  - Result: OFF now repaints with every ambient haze change and sends an idempotent stop for the selected rhythm, including before QLC+ echoes a new selection. SHOW puts modes above a compact room card, effects above timed haze, and 17 rig colours beside a local GOLPES DE COLOR switch. The colour bank maps to bounded one-shot hits when the switch is on.
  - Evidence: `desk_damage_test`, `desk_layout_test`, `showmap_v2_test`, `desk_geometry_test` and the rendered `docs/evidence/show-tablet-2026-10-21.png`; isolated QLC+ 5.2.2 reported `Running` then `Stopped` after `setFunctionStatus|639|0`.

- [x] 2026-10-19 — **Request channel after an idle supplicant restart:** v0.1.3 redials
  only when an abandon left no socket; a daemon restarting while nothing is
  pending leaves the socket connected to the dead peer, `wpa_ctrl_begin`'s
  send fails and neither caller abandons, so every later request fails.
  - Result: wpa_ctrl_begin abandons and sends once more when its send fails; a daemon still gone fails at once and the next request dials again. wpa_ctrl_test restarts the fake with nothing pending.
  - Evidence: tools/test-dmx-desk-host.sh exit 0, 45/45 PASS on macOS and 45/45 on Linux (taq102 OrbStack, gcc 15.2 + ASan); the new wpa_ctrl_test block aborts at line 271 (exit 134) on both with the v0.1.3 wpa_ctrl_begin.c.

- [x] 2026-10-19 — **Release:** v0.1.3: tiles wear their map icon, Fijo
  only over a toggle, the request channel redials, flip endpoints fixed.
  - Result: the map's per-control `icon` drawn as 22 vector marks on 111
    Vibra tiles (fbf61c3); SHOW's Fijo heading only over a placed toggle
    (cb5df70); `wpa_ctrl_begin` redials once per request after a failed
    abandon (f5dd224); `touch_flip_value` returns `width - 1 - value` and
    the desk logs `flip none` (266de97). Tagged `v0.1.3` (f5dd224).
  - Evidence: `tools/test-dmx-desk-host.sh` exit 0, 45/45 PASS on macOS and
    45/45 on Linux (taq102 OrbStack, gcc + ASan), geometry audit
    `failures=0 warnings=0`; taq102 `tools/build-dmxdesk.sh` cross-build
    (GCC 14.3.0, -Werror) produced an ARM ELF.

### 2026-09

- [x] 2026-09-27 **v0.1.2: releases return the family to its floor.** The desk
  presses the hook a pick's `releaseTo` names for the room state that is on
  (b17b6ec, 64560c7), toggles are queued 200 ms apart with stop-all emptying
  the queue first (ca9efd1, da3fd1e) and logged (7628bda). The map is the one
  qlctool v0.1.9 builds for DMX-Fixtures a8f2430 (99 picks, 432 entries), and
  `vc-vibra.json` was recaptured from QLC+ 5.2.2 on :9997 (583 widgets, every
  map control present). Evidence: `tools/test-dmx-desk-host.sh` 43/43 on
  macOS and 43/43 on Linux (taq102 OrbStack); the taq102 cross-build is clean;
  tagged `v0.1.2` (e49be39).

- [x] 2026-09-26 - **The host tests pass on Linux.** Six failed under GCC 15.2
  with ASan and LeakSanitizer (OrbStack Ubuntu, aarch64) while macOS passed
  all 39: three render tests leaked their canvas, `audit/text.c` had one-line
  `if` pairs (`-Wmisleading-indentation`), `desk_power_test` built 256-byte
  paths from a 256-byte node (`-Wformat-truncation`), and `qlc_session_test`
  never heard its reply (`last_rtt` -1, not a wrong value): the fake master
  wrote a frame as header then payload, and Nagle held the payload behind the
  delayed ACK for 28-34 ms of real time, longer than the fake clock's polls.
  A seventh, `wifi_join_test`, failed intermittently: its config sat under
  `./output`, the Mac's folder over the VM's shared mount, where a failed
  join's restore (fsync included) took up to 252 ms against the 50 ms bound;
  it now lives in the script's local TMPDIR (worst 17 ms). No bound was
  loosened. Evidence: `tools/test-dmx-desk-host.sh` 39/39 on macOS and 39/39
  in three consecutive Linux runs; tagged `v0.1.1`.
