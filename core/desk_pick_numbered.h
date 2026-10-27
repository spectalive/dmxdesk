// Whether a pick reads as its number: the big digits over its caption, as
// GOBOS' "14" over "Gobo 14". A section's numbered captions are grouped by
// their stem (what comes before the number, desk_caption_number), and the
// stem most of them share wins, provided at least two do; only the picks of
// that stem are numbered. So "Gobo 1".."Gobo 17" are numbers while "Gobo
// Repartido 3" and "Gobo Shake - Gobo 9", which end in a number under
// another stem, and "Paneles Manual", which ends in none, keep their caption
// tile; PRISMA's "1", "2", "1 y 3" share the empty stem and are numbers.
// Derived from the captions alone, never from a show's names.
#ifndef DESK_PICK_NUMBERED_H
#define DESK_PICK_NUMBERED_H

#include "showmap.h"

// `control` is an index into map->control inside `sec`; returns 1 or 0.
int desk_pick_numbered(const struct show_map *map, const struct map_section *sec, int control);

#endif
