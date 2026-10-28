// A control of the surface by the console's id: NULL when the console has no
// such control, which is how a stale show map is caught.
#ifndef DESK_SURFACE_FIND_H
#define DESK_SURFACE_FIND_H

#include "desk_surface.h"

const struct desk_surface_item *desk_surface_find(const struct desk_surface *s, int id);

#endif
