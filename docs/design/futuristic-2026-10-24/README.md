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

Next: the owner chooses a direction before another QML appearance change.
