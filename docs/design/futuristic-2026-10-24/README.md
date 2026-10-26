# Futuristic DIRECTO concepts

Three visual proposals generated in ChatGPT from the current Mac Qt screenshot on 2026-10-24. They explore materials and lighting while retaining the existing control groups. No proposal has been selected or implemented.

| Direction | Preview | Surface treatment |
| --- | --- | --- |
| Current interface | [Source screenshot](current-layout.png) | Existing dark QML interface |
| Precision Glass | [Concept 1](precision-glass.png) | Smoked graphite, pale cyan edges and recessed colour pads |
| Aerospace Instrument | [Concept 2](aerospace-instrument.png) | Titanium panels, chamfered corners and illuminated hardware keys |
| Stage Console | [Concept 3](stage-console.png) | Black ceramic, violet effect accents and teal smoke controls |

## Layout review

Visual review found all seven modes, 18 colour pads in six columns and three rows, six held-effect tiles plus Humo vertical, ambient haze OFF and 1/2/4/8 minutes, Tempo Show with minus/plus/TAP, Master and red Parar todo in each concept. Their group positions follow the source screenshot.

The source is 1024x600; each generated concept is 1639x960. Button dimensions, spacing and typography vary, so these images are material references rather than exact geometry specifications. Some idle controls appear active because of their illumination. Any implementation must distinguish available, selected, held and disabled states using actual backend state.

Animation suggestions remain proposals. A tempo-synchronized pulse requires a real beat phase, and fade progress requires backend progress data; neither should be inferred from these images. Tablet rendering performance has not been measured for these treatments.

## Verification

The source Mac window passed its `AUDIT` command with `VERIFY 0`. Every concept was opened and visually compared with the source. PNG format and dimensions were checked with:

```bash
file docs/design/futuristic-2026-10-24/*.png
```

On 2026-10-26 the owner chose Stage Console, and the Qt desk was brought to it region by region against the concept scaled to 1024x600.

- Geometry: DIRECTO now ends its panels and Parar todo at 576 (`DESK_DIRECT_*` in `src/desk_layout.h`), the rig rows sit on the concept's 73 px pitch, the master ends level with the effects panel, and the haze rows keep the 48 px touch minimum with an 8 px bottom pad. The backend owns these rectangles, so touch follows the drawing.
- Materials: every decoration is baked once by `qt/bake_stage_chrome.py` from `qt/chrome-baker/` into 9-slice ICO images (`qt/qml/chrome/`) and drawn as one `StageChrome` per control. Each look sets a rim that fades from the top edge to a per-look side level and a coloured base light, and may add a halo, a recess frame or a dark inner line. Live colours stay in QML: the glass gradient on each tile, and a ring in each swatch's own colour.
- Type and icons: labels follow the concept's regular weight, and the icon set is drawn from `tools/icons.py` vectors at 48 px.

`docs/evidence/qt-desk-stage-console-2026-10-26.png` is the Mac capture, and `docs/evidence/qt-desk-stage-console-vs-concept-2026-10-26.png` stacks the concept (top) over it. A blurred mean per-pixel difference against the concept fell from 18.4 to 9.9 over these passes, with every region now between 7.5 and 12.4. What remains is the concept's painted texture and soft text glow, which the tablet's renderer cannot afford as live effects.
