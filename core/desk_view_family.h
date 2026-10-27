// EFECTOS' sub-navigation: the target for family `slot` of `families`, down
// the left of the content. Each is 64 tall with 8 px gaps, shorter when that
// many would not fit, so every family keeps a target whatever the map holds.
#ifndef DESK_VIEW_FAMILY_H
#define DESK_VIEW_FAMILY_H

#include "desk_view.h"

struct desk_rect desk_view_family(int slot, int families);

#endif
