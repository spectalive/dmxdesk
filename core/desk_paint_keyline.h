// A tile's frame: a rounded rectangle in `fill` inside a 1 px keyline in
// `line`. A `line` equal to `fill` draws the plain fill, which is how a
// running tile drops its keyline.
#ifndef DESK_PAINT_KEYLINE_H
#define DESK_PAINT_KEYLINE_H

#include <stdint.h>

#include "canvas.h"

void desk_paint_keyline(struct canvas *c, int x, int y, int w, int h, int r, uint32_t fill,
                        uint32_t line);

#endif
