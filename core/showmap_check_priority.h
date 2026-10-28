#ifndef SHOWMAP_CHECK_PRIORITY_H
#define SHOWMAP_CHECK_PRIORITY_H

#include "showmap.h"
#include "desk_surface.h"

// NULL when the master owns the Flash deadline and the mapped source matches.
const char *showmap_check_priority(const struct map_control *control, const struct desk_surface *doc);

#endif
