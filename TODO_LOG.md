# TODO Log

> Searchable record of closed work. Active work lives in `TODO.md`.

## 2026

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
