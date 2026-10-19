// EFECTOS' sub-navigation: one target per family down the left, its mark and
// its page's title in capitals, the family on screen in accent.
#ifndef DESK_PAINT_FAMILIES_H
#define DESK_PAINT_FAMILIES_H

#include "canvas.h"
#include "desk_fonts.h"
#include "desk_model.h"

void desk_paint_families(struct canvas *c, const struct desk_model *model,
                         const struct desk_fonts *fonts);

#endif
