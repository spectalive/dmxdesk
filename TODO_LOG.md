# TODO Log

> Searchable record of closed work. Active work lives in `TODO.md`.

<!-- tadu:log v1 since 2026-10-12 -->

## 2026

### 2026-11

- [x] 2026-11-04 — **Design:** **Finish the Qt Quick desk trial on the physical tablet.**
  On 2026-11-02 the trial was brought to HEAD for the owner to see Stage Console on the tablet: `/data/qt-full/qml` is `qt/qml` at 0344440 (previous kept as `qml.2026-10-27`), `dmxdesk-backend` is the installed desk 84775a35 (previous kept as `dmxdesk-backend.2026-10-26`); `qt-desk` itself was unchanged because no Qt C++ changed since its 2026-10-26 build. Launched with `run-qt-desk-trial.sh`, linked to 192.168.1.87, and a `--capture` BMP passed `VERIFY 0`; this Qt saves BMP only, a `.png` capture fails with exit 1. The launcher returns to the C desk after 30 minutes.
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
  On 2026-11-03 a Mac pass at HEAD (backend built in Alpine, mock master) captured Directo, Efectos with all six families and the GOLPES pages, Ritmo, Ajustes, the network scan and the IP keyboard: every capture VERIFY 0. The settings sheet was the one page still in flat grey panels with two button styles; it now has the Stage Console floor, panels and key face with its touch rectangles unchanged (115c388, `docs/evidence/qt-desk-settings-stage-2026-11-03.png`); ctest 8/8.
  On 2026-11-04 the physical panel was brought to HEAD without a Buildroot rebuild: only QML changed since v0.1.11, and the installed `/usr/bin/dmxdesk` (v0.1.11) already speaks `--qt-pipe`, so `/data/qt-full/qml` is now b82128e (previous kept as `qml.5b3541a`) and `/data/qt-full/dmxdesk-backend` is a copy of the installed v0.1.11 desk (previous kept as `dmxdesk-backend.2026-11-02`). Against the Mac mock master, `qt-desk --agent-stdio` captured Directo, Efectos, Ritmo and Ajustes (opened from the gear) on the EGLFS panel, every capture VERIFY 0 (`docs/evidence/qt-desk-settings-tablet-2026-11-04.png`), and `tests/qt_pipe_integration.py` passed with the tablet's backend over SSH. The first capture showed "Enlazado" beside a red dot: the mock dropped the websocket whenever the desk was quiet for 0.5 s, so the link flapped every ~2 s and the dot's 160 ms colour change never finished; fixed in `tests/qt_mock_handler.py`, guarded by `qt-mock-idle` (fails on the old handler), ctest 9/9. Earlier mock-based frame-time runs saw these same closes. Still seen on the panel: right after link-up, colour and opacity motions take about 2 s to settle (dot red at 6 s, teal at 8 s), because the first linked frames are slow. Multi-touch through the Qt EGLFS touch plugin itself: evdev events written to `/dev/input/event0` (hold FLASH at 605,225 with slot 0, tap PARAR TODO at 955,505 with slot 1, release slot 0; raw = screen / 0.615x0.670) reached the mock as `12|255` (FLASH's `priorityWidget`), `20|255` (`stopAll`) and `12|0`. The C desk relinked to the mini's QLC+ after every run.
  - Result: The Qt desk at HEAD (b82128e QML, v0.1.11 backend) runs on the physical 1024x600 panel: every page and the settings sheet capture VERIFY 0, and two-finger stop-all and held-Flash release work through the EGLFS touch plugin.
  - Evidence: 2026-11-04 tablet runs: agent captures VERIFY 0 (docs/evidence/qt-desk-settings-tablet-2026-11-04.png); injected evdev gesture gave 12|255, 20|255, 12|0 at the mock; qt_pipe_integration.py passed with the tablet backend; ctest 9/9 incl. qt-mock-idle.

- [x] 2026-11-03 — **Design:** **Release v0.1.11 and flash it to the tablet.** Since 2026-11-01 the tablet runs `main` only from RAM (`/usr/bin/dmxdesk` and `/usr/share/dmxdesk/vibra.desk.json` replaced, backups `/tmp/dmxdesk-prev` and `/tmp/vibra.desk.json-prev`); a reboot restores v0.1.10 and its old map, which no longer matches the regenerated `Vibra.qxw` the mini's QLC+ now loads (prism and pixel picks renamed and reordered), so after a reboot reinstall `main` the same way before using PRISMA. Blocked: the owner decides when to release (owner, 2026-11-01). Then: tag v0.1.11 here, bump `DMXDESK_VERSION` in spectalive/taq102 `br2-external/package/dmxdesk/dmxdesk.mk`, build the image, flash in loader mode with the owner present.
  - Result: dmxdesk v0.1.11 tagged at d50fa83 and pinned in spectalive/taq102; the v99 image runs it from boot and recovery, so a reboot keeps main's desk and the regenerated map.
  - Evidence: taq102 v99 (SHA256 0a02be0d...) flashed 2026-11-03, boot and recovery readbacks matched; after reboot /usr/share/dmxdesk/VERSION names d50fa83 clean, map 1192f207..., and the desk logged link ready (linked)

- [x] 2026-11-03 — **Core:** **Decide how the desk's burst holds reach QLC+ over OSC.** Since spectalive/qlctool v0.2.6 (2026-10-28) a show generated with `[controllers] osc = "<prefix>"` binds every button, slider and speed dial to `<prefix>/<widget id>`, but the burst holds are functions with no widget, so the OSC adapter has nothing to press for them. Next: decide whether the generator gives each burst function a button (on a hidden page they would ignore external input, `VCPage::inputValueChanged`) or the OSC surface leaves bursts out.
  - Result: Bursts reach QLC+ over OSC as presses of their priority Flash widget, which every generated map names; no extra buttons are generated. The function-only path stays for hand-written maps through a surface 'functions' entry. A burst whose priority Flash is on a hidden page is disabled by the existing hidden-page rule.
  - Evidence: tests/dmx-desk/osc_vibra_test.c: all 25 enabled Vibra bursts have priority_widget_id > 0 and encode as /vibra/<widget> Flash messages; full host suite (tools/test-dmx-desk-host.sh) green

- [x] 2026-11-02 — **Event socket after a supplicant restart during a pending request:** an abandon whose dial reaches the new daemon at once never re-ATTACHes, so scan results stop arriving.
  - Result: wpa_ctrl_abandon re-attaches the event socket whenever its dial succeeds, since a timeout cannot be told from a restart. wpa_ctrl_reattach sends DETACH on the old event socket first, so a daemon that did not restart drops the old address, and moves the event socket to a fresh per-generation address, so that DETACH's reply never reaches the new socket.
  - Evidence: 2026-11-02: wpa_ctrl_test restarts the fake while LATE is pending, abandons, and expects a scan's results event; without the fix it fails at line 325, with it it passes 3/3. Full host suite tools/test-dmx-desk-host.sh: EXIT 0, 62 PASS, no FAIL; gcc -Wall -Wextra -Werror clean on reattach, begin, abandon and event in alpine:3.22.

- [x] 2026-11-02 — **Event socket after a supplicant restart:** `wpa_ctrl_open` ATTACHes
  the event socket once; a restarted wpa_supplicant forgets that, and
  `wpa_ctrl_event` only ever sees EAGAIN, so `CTRL-EVENT-SCAN-RESULTS` never
  queues the card's results and a join stops hearing events (found
  2026-10-19 reviewing v0.1.4, untested). Next: in `wpa_ctrl_test`, restart
  the fake and expect a SCAN's event; re-dial and re-ATTACH the event socket
  when a request had to redial.
  - Result: A request that had to dial the request socket again (a redial, or a send to a dead peer) now also dials the event socket and sends ATTACH without waiting (platform/linux-fb/wpa_ctrl_reattach.c); wpa_ctrl_event drops that ATTACH's unprefixed OK. Not covered: a daemon that restarts while a request is pending and whose abandon redials at once.
  - Evidence: 2026-11-02: wpa_ctrl_test asks for a scan after each simulated restart and expects its results event (stale card events drained first); it fails at scan_event without the fix and passes with it 3/3. Full host suite tools/test-dmx-desk-host.sh: EXIT 0, 62 PASS, no FAIL; gcc -Wall -Wextra -Werror clean on the three changed files in alpine:3.22.

- [x] 2026-11-02 — **Tests:** **Make the Qt pipe integration map requirement explicit.** On 2026-10-24, the installed backend did not link when the command omitted its map; adding `--map /usr/share/dmxdesk/vibra.desk.json` passed tabs, colour-hit mode, two-finger Stop All and EOF Flash release. Next: expose a map argument in `tests/qt_pipe_integration.py` and preserve redacted backend diagnostics when linking fails.
  - Result: tests/qt_pipe_integration.py requires --map and passes it to the backend; when the desk does not link, the assertion carries the backend's last stderr lines with the connect host and IPv4 addresses replaced (tests/qt_backend_diagnostics.py).
  - Evidence: 2026-11-02, backend built in alpine:3.22 and run through a docker wrapper against the mock master: --map show/vibra.desk.json passed linked, tabs, colour-hit, two-finger stop-all and EOF release; --map /nope.json failed with 'map: cannot open /nope.json'; no --map is an argparse error; redaction checked on a sample with an IP and a hostname.

### 2026-10

- [x] 2026-10-28 — **Packaging:** **Update the taq102 package for the core/platform split before the next version bump.** On 2026-10-27 `src/` became `core/` and `platform/linux-fb/`, and the source list moved to `dmxdesk.sources` at the repository root with paths. spectalive/taq102's `br2-external/package/dmxdesk/dmxdesk.mk` still builds `$(@D)/src/` with `-I$(@D)/src`, which is right for the pinned release but fails on any tag after the split. Next: when bumping, change it to `-I$(@D)/core -I$(@D)/platform/linux-fb` and `$(addprefix $(@D)/,$(shell cat $(@D)/dmxdesk.sources))`. The Buildroot cross-compiler already links the split tree, and its stripped binary is byte-identical to the pre-split one.
  - Result: No change is left for the bump beyond DMXDESK_VERSION and the hash.
  - Evidence: spectalive/taq102 1d5b60c: dmxdesk.mk and tools/build-dmxdesk.sh accept both layouts; pinned v0.1.10 rebuilds byte-identical (d68f4720...), dmxdesk main ecdeb7a cross-builds under -Werror with Buildroot GCC 14.3.0

- [x] 2026-10-28 — **Core:** **Have the show generator map the Virtual Console to OSC.** On 2026-10-28 `tools/qlc-osc-surface` began writing the desk's OSC surface file from QLC+'s `/vc.json`, together with the table of external inputs QLC+ must have (`--channels`: widget, input id, CRC-16 channel, path). The workspace side, an OSC input and feedback patch on universe 1 and those 518 widget inputs plus 18 dial multiplier inputs for Vibra, was done in the trial by patching a copy of the `.qxw` in a scratch script, which is not in any repository. Next: teach spectalive/qlctool to emit the OSC inputs from the same rule (path `<prefix>/<widget id>`, channel = QLC+'s `Utils::getChecksum` of it, dial multipliers on inputs 4 and 6-13), so an OSC-driven show is generated like the others. Also, the burst holds have no widget to press over OSC: decide whether the generator gives each burst function a button.
  - Result: The generator emits the OSC input patch and bindings from the converter's rule; an osc_control_bound_twice check guards CRC-16 collisions. Feedback stays an Inputs/Outputs setting.
  - Evidence: spectalive/qlctool v0.2.6 (2ed0ba3): [controllers] osc binds every Button/Slider/SpeedDial to <prefix>/<id> and dial multipliers on inputs 4 and 6-13 of an OSC universe; codeality-py gate exit 0 (1037 passed); vibra-lighting vibra_compare --validate: three workspaces identical, 0 findings, QLC+ loaded them

- [x] 2026-10-28 — **Core:** **Try the OSC adapter against a real console.** On 2026-10-28 `core/osc_console.c` joined QLC+ as a second adapter (`dmxdesk --osc FILE`), proven on loopback and in `--qt-pipe` mode against a UDP socket playing the console: the Vibra gesture script sends the same controls in the same order as over QLC+, with the same 41 controls enabled. No real OSC console has received it yet, and the press-as-1-then-0 and the 0..1 grand master are the common convention, not any one console's. Next: write a surface file for one console that speaks OSC (QLC+'s own OSC input plugin on the headless engine at :9997 needs no venue hardware) and check each kind of control moves there.
  - Result: QLC+ 5.2.2's OSC input on loopback drives buttons, solo frames, flashes, stop-all, dial time and multipliers through the adapter, with QLC+'s feedback heard; msRange, factorAddresses and per-item feedback added for what it needed; hidden-page widgets documented as disabled
  - Evidence: 9efe979. Trial on a headless QLC+ on :9996 with the Vibra workspace mapped to /vibra/<id>: toggle/solo/flash/stop states and feedback events match, dial 504->501 ms, 700->698 ms, factors 1/16x..16x set; host suite 59/59, ARM -Werror clean

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
