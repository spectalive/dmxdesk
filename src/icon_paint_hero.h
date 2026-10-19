// A map glyph's mark at 72 px, the picture in the SALA AHORA hero, baked by
// tools/icons.py beside the smaller sets and blended in any colour.
#ifndef ICON_PAINT_HERO_H
#define ICON_PAINT_HERO_H

#include <stdint.h>

#include "canvas.h"
#include "icon_data.h"

// Blends the hero mark of icon `id` with its top-left at (x, y). Returns 0
// when it drew, -1 when the icon has no hero mark and nothing was drawn.
int icon_paint_hero(struct canvas *c, enum icon_id id, int x, int y, uint32_t colour);

#endif
