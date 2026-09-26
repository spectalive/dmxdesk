# TODO

The desk's open work from before the split (2026-09-26) is still in
spectalive/taq102's `TODO.md` under "DMX desk", because almost all of it needs
the tablet. Items here are about this repository on its own.

## Tests

- [ ] Six host tests fail on Linux (Ubuntu, GCC 15.2, aarch64, ASan with
  LeakSanitizer on), while all 39 pass on macOS, where LeakSanitizer is off.
  Measured 2026-09-26 by running `tools/test-dmx-desk-host.sh` at the split
  commit in an OrbStack Ubuntu machine: `icon_test`, `setup_render_test` and
  `speed_render_test` leak a test-side canvas; `desk_geometry_test`
  (`-Wmisleading-indentation` in `tests/dmx-desk/audit/text.c`) and
  `desk_power_test` (`-Wformat-truncation`) stop at `-Werror`;
  `qlc_session_test` fails `qlc_session_last_rtt(s) == 514` at line 278.
  Smallest next step: free the canvases, split the one-line `if`s, size the
  buffer, then read why the RTT differs under glibc.
