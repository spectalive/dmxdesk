// A heads' trajectory mark: one of the 40 px alpha bitmaps tools/icons.py
// bakes beside the 24 px icons, blended into a canvas in any colour.
#ifndef ICON_PAINT_LARGE_H
#define ICON_PAINT_LARGE_H

#include <stdint.h>

#include "canvas.h"
#include "icon_data.h"

// Blends one mark with its top-left at (x, y); an id out of range draws nothing.
void icon_paint_large(struct canvas *c, enum icon_large_id id, int x, int y, uint32_t colour);

#endif
