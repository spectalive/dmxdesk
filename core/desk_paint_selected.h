// The selected look, for what runs and for the tab or family on screen: a
// frosted fill a step lighter than a tile, inside a brighter 1 px hairline
// whose top edge is lighter still, as light catches the top of glass.
#ifndef DESK_PAINT_SELECTED_H
#define DESK_PAINT_SELECTED_H

#include "canvas.h"

void desk_paint_selected(struct canvas *c, int x, int y, int w, int h, int r);

#endif
