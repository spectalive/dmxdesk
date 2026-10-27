// An icon at 36 px: the same vector as the 24 px one, baked larger by
// tools/icons.py for DIRECTO's states and holds, blended in any colour.
#ifndef ICON_PAINT_MID_H
#define ICON_PAINT_MID_H

#include <stdint.h>

#include "canvas.h"
#include "icon_data.h"

// Blends one icon with its top-left at (x, y); an id out of range draws nothing.
void icon_paint_mid(struct canvas *c, enum icon_id id, int x, int y, uint32_t colour);

#endif
