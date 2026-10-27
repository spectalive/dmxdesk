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

- Geometry: DIRECTO now ends its panels and Parar todo at 576 (`DESK_DIRECT_*` in `core/desk_layout.h`), the rig rows sit on the concept's 73 px pitch, the master ends level with the effects panel, and the haze rows keep the 48 px touch minimum with an 8 px bottom pad. The backend owns these rectangles, so touch follows the drawing.
- Materials: every decoration is baked once by `qt/bake_stage_chrome.py` from `qt/chrome-baker/` into 9-slice ICO images (`qt/qml/chrome/`) and drawn as one `StageChrome` per control. Each look sets a rim that fades from the top edge to a per-look side level and a coloured base light, and may add a halo, a recess frame or a dark inner line. Live colours stay in QML: the glass gradient on each tile, and a ring in each swatch's own colour.
- Type and icons: labels follow the concept's regular weight, and the icon set is drawn from `tools/icons.py` vectors at 48 px.

`docs/evidence/qt-desk-stage-console-2026-10-26.png` is the Mac capture, and `docs/evidence/qt-desk-stage-console-vs-concept-2026-10-26.png` stacks the concept (top) over it. A blurred mean per-pixel difference against the concept fell from 18.4 to 9.9 over these passes, with every region now between 7.5 and 12.4. The floor then took the concept's leather grain (`qt/bake_floor_grain.py`, matched by contrast: standard deviation 3.4 against the concept's 3.9), which reads closer but scores 10.1, because random grain cannot coincide pixel for pixel. Text halos are cheap `Text.Outline` strokes rather than blurs. What remains is painted detail in the generated image (stray reflections, the exact grain) that a live UI does not reproduce.

The owner then had ChatGPT export the concept's elements separately (`elements/`): `floor.png` is the floor without any control, `tiles.png` and `keys.png` every tile and key in its own state with rim light and glow but no text, and `panels.png` the empty group panels. `qt/export_stage_floor.py` writes the floor as one window-sized BMP (the tablet's Qt reads BMP without a plugin) dimmed to 80%, and `qt/slice_stage_elements.py` cuts each element out with its glow, keeps only pixels nearer that element than any neighbour on the sheet, scales it to the desk and reduces it to a 9-slice ICO. The live tiles, the top tabs, the selected haze OFF button, the tempo panel and keys, Humo vertical's wide tile, and the status and rig panels now use these (`keys.png` added the tabs and OFF; its tempo key and Parar todo scored worse than `tiles.png`'s and are not used); the effects and haze panels keep the baked looks, which matched better. The sheets draw a dark bezel outside each tile's rim light, while the concept puts the rim on the tile's edge, so tile and key crops start 5 sheet pixels inside the bezel and leave it to the glow margin. The sheets also spill more glow into the gaps than the concept, whose gaps stay nearly black, so the tile sheet's halo beyond the bezel is kept at 25%; the selected haze button is lifted 1.5x to the concept's lighter glass, and the effect icons are 23 px as drawn there. The blurred difference fell from 10.3 to 8.7. A colourless swatch overlay (bezel, gloss and edge light to lay over the live colour) was also exported and tried; it veiled the colours and lit the bezel silver where the concept's is dark and tinted, scoring 10.9 to 12.6 on the rig against 8.9 for the baked swatch looks, so the swatches keep those. The tile scale (0.38) was chosen by eye: smaller scales score lower on the blurred metric (9.25 at 0.27) but square the corners and dim the rims against the concept.

A later pass measured the remaining gap. The ambient haze tiles' glass was brighter than the concept's, so `fill-quiet` is sliced in its own group at 68% brightness (interior 46,51,60 against the concept's 47,53,66), and their cloud and minutes label move 3 and 2 px right and the cloud 2 px up to sit where the concept draws them. The effect icons grow from 20 to 23 px, matching the concept's ink within 1-3 px. Haze fell from 11.6 to 8.1 and the whole desk from 8.7 to 8.3. Tried and rejected: unsharp-masking the tile sheets to sharpen the rim line (8.9 overall), a darker or lighter OFF than 1.5x, the tile halo at 10% or 0% (no change), dropping the halo's 2 px bezel (worse), contrast on the hold looks (no change), the lit hold look for idle effects (22.4 on effects) and 25 px effect icons. What keeps effects at 10.4 is the rim itself: the concept draws a hard one-pixel line at luminance 128-220 with dark glass right inside it, where the scaled sheet gives a soft band near 95.
