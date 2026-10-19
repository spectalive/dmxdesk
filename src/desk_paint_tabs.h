// The bar's three tabs as pills: the one on screen in accent with white
// words, the others dark. A tab with no page is not drawn; with the settings
// open none reads as current, since none can be pressed.
#ifndef DESK_PAINT_TABS_H
#define DESK_PAINT_TABS_H

#include "canvas.h"
#include "desk_fonts.h"
#include "desk_model.h"

void desk_paint_tabs(struct canvas *c, const struct desk_model *model,
                     const struct desk_fonts *fonts);

#endif
