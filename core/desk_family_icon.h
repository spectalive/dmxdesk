// A family's mark in EFECTOS' sub-navigation. The page keys are the ones
// `qlctool deskmap` writes for every show, so the families it knows get a
// drawn mark of their own; any other page wears its first control's glyph,
// or none (-1).
#ifndef DESK_FAMILY_ICON_H
#define DESK_FAMILY_ICON_H

#include "showmap.h"

int desk_family_icon(const struct show_map *map, int page);

#endif
