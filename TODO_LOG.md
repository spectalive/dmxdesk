# TODO Log

> Searchable record of closed work. Active work lives in `TODO.md`.

<!-- tadu:log v1 since 2026-10-12 -->

## 2026

### 2026-10

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
