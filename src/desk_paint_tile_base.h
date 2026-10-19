// A flowed tile's ground, one look for the picks that carry no colour: the
// accent fill while the master says it runs, the 1 px keyline otherwise, the
// glass while it cannot be chosen; an ink outline while a finger is on it,
// and the pending dot while its frame is unanswered. Returns the ink its
// words and mark take on that ground.
#ifndef DESK_PAINT_TILE_BASE_H
#define DESK_PAINT_TILE_BASE_H

#include <stdint.h>

#include "canvas.h"
#include "desk_model.h"

uint32_t desk_paint_tile_base(struct canvas *c, const struct desk_control *ctl,
                              const struct desk_placement *p, enum desk_link link);

#endif
