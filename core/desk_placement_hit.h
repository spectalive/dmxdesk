// Whether a point lands on placement `p`: inside desk_placement_target.
#ifndef DESK_PLACEMENT_HIT_H
#define DESK_PLACEMENT_HIT_H

#include "desk_placement.h"

int desk_placement_hit(const struct desk_placement *p, int x, int y);

#endif
