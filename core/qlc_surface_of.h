// QLC+'s /vc.json document as a desk surface: every widget becomes an item
// under its own id, in the same order, with what the desk reads from it.
// Returns 0, or -1 when out of memory, leaving `out` empty.
#ifndef QLC_SURFACE_OF_H
#define QLC_SURFACE_OF_H

#include "desk_surface.h"
#include "vcjson.h"

int qlc_surface_of(const struct vc_doc *doc, struct desk_surface *out);

#endif
