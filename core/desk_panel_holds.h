// Whether placement `p` lies inside panel `h`: on its page and bank, its
// corner within the panel's field.
#ifndef DESK_PANEL_HOLDS_H
#define DESK_PANEL_HOLDS_H

#include "desk_layout_resolve.h"

int desk_panel_holds(const struct desk_heading *h, const struct desk_placement *p);

#endif
