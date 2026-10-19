# UI brief: dmxdesk (the Vibra tablet desk)

## What the app is for

A touch lighting desk on a 10" tablet that runs the Vibra events' light show.
The show itself runs in QLC+ on a Mac (AUTO lights the room unattended); the
tablet is how a person in the room changes what the lights are doing when
something happens - a speaker takes the stage, the room goes quiet, the party
peaks, the night ends - without touching the Mac. Every button presses a real
QLC+ console control over the network; one the console cannot back is shown
disabled with the reason.

## Who uses it

- **Whoever is nearest at an event** (Vibra staff, the DJ, the owner): not a
  lighting operator. They pick the room's mood in one tap, fire a flash or a
  burst of smoke while holding, and stop everything if something goes wrong.
  Used for seconds at a time, in a dark room, often standing, sometimes in a
  hurry. The owner's words: "lo que necesita un operador es tenerlo todo claro".
- **The owner setting up a night**: picks colours, moving-head figures, gobos,
  prism and speed per page before or during the show. Knows the rig, not
  lighting software.

## Screens

Fixed 1024x600 landscape, dark room, fingers only. A top bar (page tabs, link
status, Wi-Fi, battery, settings, lock) and a right rail always visible (MASTER
fader, PARAR TODO). Pages:

- `SHOW`: the room state (AUTO, CHARLA, TRANQUILO, FIESTA, LOCURA, BLANCO
  TOTAL, TODO NEGRO - one at a time), GOLPES (flash/strobe while held, with a
  maximum time), HUMO A MANO (smoke while held), AMBIENTE (timed smoke, OFF
  stops it), COLOR DEL RIG (one-tap colours), TEMPO SHOW with TAP. The page
  used most.
- `COLOR`: colour programs (AUTO) and fixed rig colours, in banks (1/2, 2/2,
  GOLPES) chosen at the bottom.
- `PIXELES`, `CABEZAS`, `GOBOS`, `PRISMA`: the same shape for pixel effects,
  moving-head figures, gobos and prism: an AUTO row plus a grid to choose from,
  in banks.
- `CONTROL`: intensity sweeps and the vertical smoke light.
- `SPEED`: two tempos (show and movement) with TAP, +/-1 BPM, x2 slower/faster,
  TAP both.
- Settings (gear): Wi-Fi, the master's address, a confirm and an on-screen
  keyboard. The lock: a tap locks, a 1 s hold on the padlock unlocks; the power
  key blanks the screen.

## Constraints

- No web stack: the desk draws itself in C on the framebuffer. One typeface
  (Inter), flat fills, rounded rectangles, line icons; no images, gradients or
  shadows beyond what a rectangle fill can do. Keep it cheap to draw.
- Touch targets at least 48 px; nothing that needs hover, swipe or precision.
- Every control on the current pages exists in the show and must stay; labels
  come from the show map (Spanish), and can be long ("Diamante Simultaneo").
- Dark theme only (a dark venue); PARAR TODO must stay unmistakable and always
  reachable. The colour tiles show the real colour.
- States the desk already knows and should show: which room state / program is
  running, a hold in progress (with its maximum time), disabled with a reason,
  link up or down, locked.
