# v3 redesign: what each mockup asks for

The target is the five mockups in this directory (`directo.png`,
`efectos-color.png`, `efectos-cabezas.png`, `efectos-gobos.png`, `ritmo.png`),
drawn by ChatGPT from `docs/ui-brief.md` and refined by the owner on
2026-10-19; `critique.md` is the first-round critique with the visual system.
Every line is `build`, `exists`, `needs schema` or `rejected` with the reason.

## Whole app

- Three top tabs DIRECTO / EFECTOS / RITMO replacing the eight page tabs - build
- Status cluster: "enlace" word + green dot, Wi-Fi, battery %, gear, lock - build (dot new; rest exists)
- Right rail: MASTER label, big %, vertical fader; PARAR TODO red with "1.0 s fade" - exists, restyle
- Flat dark panels grouping each section, 1 px keylines, 12 px radius - build
- Active selection = solid accent-blue fill (replaces amber) - build
- Section headings in caps, muted, inside their panel - build
- Pager as full-width pills at the bottom, current one white - exists, restyle
- Conceptual mockup label - rejected: annotation, not UI
- Gradients, glows, shadows - rejected: brief forbids them; none in the round-3 set

## DIRECTO (`directo.png`)

- SALA AHORA panel: running state name large, its detail under it - build
- Seven state tiles in the same panel, icon + name, detail on the running one - build
- GOLPES / MIENTRAS PULSES: all seven holds in one row (FLASH, FLASH LENTO, FLASH COLOR, HUMO YA, HUMO VERT, STROBO, STROBO SUAVE) - build
- Hold countdown / "pulsa · máx N s" foot - exists; the mockup drops it, kept because the brief requires a hold's maximum time
- HUMO AMBIENTE: OFF + cada 1/2/4/8 min as one segmented row - exists, restyle
- COLOR DEL RIG: ten swatches without names, running one ringed - build
- TEMPO SHOW: big BPM, "BPM base", TAP - exists, restyle
- Equalizer bars - rejected: the tablet hears no audio
- Pulse at the show tempo - rejected for now: new, no clock on the desk; TODO

## EFECTOS (`efectos-color.png`, `efectos-cabezas.png`, `efectos-gobos.png`)

- Left sub-navigation: COLOR, PIXELES, CABEZAS, GOBOS, PRISMA, CONTROL with icons - build
- Each family: AUTO panel, ELEGIR panel with n/m banks, GOLPES bank where it exists - exists (flow), restyle
- COLOR picks as large colour squares with the name under - build
- CABEZAS picks with the trajectory drawn (Circulo, Ocho, Linea, Diamante, Cuadrado, Hoja, Lissajous; Simultaneo and Alternado variants) - build
- GOBOS picks as a big number with the name under ("Gobo 14") - build
- GOBOS page title + one-line description - rejected: the map carries no description; title is the sub-nav
- Footer note listing other families - rejected: mockup annotation

## RITMO (`ritmo.png`)

- Two equal cards TEMPO SHOW / VEL. MOVIMIENTO, side by side - build
- Big BPM, "BPM base", "Tiempo x1" - exists, restyle
- Wide Tap, then -1 / +1 BPM, then Más lento x2 / Más rápido x2 - exists, relayout
- TAP ambos full width below - exists
- "nuevo" pulse dot - rejected for now: same as DIRECTO's pulse
