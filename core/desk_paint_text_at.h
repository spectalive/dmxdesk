// One line of text with its top-left at (x, y), fitted to `max_w`: with the
// font when there is one, with the 3x5 glyphs when there is not.
#ifndef DESK_PAINT_TEXT_AT_H
#define DESK_PAINT_TEXT_AT_H

#include <stdint.h>

#include "canvas.h"
#include "font.h"

void desk_paint_text_at(struct canvas *c, struct font *f, int x, int y, int max_w, const char *s,
                        uint32_t col);

#endif
