// The tile a LIVE accent wears in GOLPES: a fog hit keeps the fog field, any
// other hit the hold look, and an accent that is a plain toggle stays a cue.
#ifndef DESK_SHOW_ACCENT_TILE_H
#define DESK_SHOW_ACCENT_TILE_H

#include "desk_placement.h"
#include "showmap.h"

enum desk_tile desk_show_accent_tile(const struct map_control *c);

#endif
