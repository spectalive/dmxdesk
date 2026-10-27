// Appends one placement to SHOW's page. Returns 0, or -1 when the layout
// holds no more.
#ifndef DESK_SHOW_PLACE_H
#define DESK_SHOW_PLACE_H

#include "desk_layout_resolve.h"

int desk_show_place(struct desk_layout *out, int control, int page, int x, int y, int w, int h,
                    enum desk_tile tile);

#endif
