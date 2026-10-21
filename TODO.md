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
  selected swatch also uses its outline without a check mark. Next: resume
  the preserved Buildroot build and verify remaining tablet gestures before
  considering any default-interface change.
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
