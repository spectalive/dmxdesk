// A surface into out, items and all; out owns its own copy. 0, or -1 when
// memory runs out, which leaves out empty.
#ifndef DESK_SURFACE_COPY_H
#define DESK_SURFACE_COPY_H

#include "desk_surface.h"

int desk_surface_copy(const struct desk_surface *src, struct desk_surface *out);

#endif
