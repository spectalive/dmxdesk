// One line of text centred in a box, fitted to its width: with the font when
// there is one, with the 3x5 glyphs when there is not.
#ifndef DESK_PAINT_TEXT_CENTRED_H
#define DESK_PAINT_TEXT_CENTRED_H

#include <stdint.h>

#include "canvas.h"
#include "font.h"

void desk_paint_text_centred(struct canvas *c, struct font *f, int x, int y, int w, int h,
                             const char *s, uint32_t col);

#endif
