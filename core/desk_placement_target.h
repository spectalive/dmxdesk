// The rectangle placement `p` takes touches in. A tile takes the touches
// inside the rectangle it is drawn in, except an icon switch: its 28 px square
// is a 6 mm target, so it takes them over a 44 px square centred on it, the
// margin that the heading row's padding and the gap above the swatches leave
// empty.
#ifndef DESK_PLACEMENT_TARGET_H
#define DESK_PLACEMENT_TARGET_H

#include "desk_placement.h"
#include "desk_view.h"

struct desk_rect desk_placement_target(const struct desk_placement *p);

#endif
