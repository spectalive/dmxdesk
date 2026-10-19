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

Nothing open.

## Design

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
