// Keeps only the releaseTo entries the map itself vouches for.
#ifndef SHOWMAP_RELEASE_TO_PRUNE_H
#define SHOWMAP_RELEASE_TO_PRUNE_H

#include "showmap.h"

// Drops, with a line on stderr, every releaseTo entry whose key is not the
// widget of a state control, or whose value is not the widget of a hook in
// the same solo frame as the control carrying it (a control in no solo
// frame keeps none). The rest of the map is untouched.
void showmap_release_to_prune(struct show_map *map);

#endif
