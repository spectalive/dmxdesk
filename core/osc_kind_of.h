// A surface file's kind word as the desk's control kind. -1 for a word the
// desk does not know.
#ifndef OSC_KIND_OF_H
#define OSC_KIND_OF_H

#include "desk_surface.h"

int osc_kind_of(const char *word, enum desk_surface_kind *out);

#endif
