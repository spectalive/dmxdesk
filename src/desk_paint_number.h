// A numbered pick (desk_pick_numbered): its number large, its whole caption
// small under it, "14" over "Gobo 14"; a caption that is the number alone
// ("1 y 3") is drawn once, large. Running, the accent fill.
#ifndef DESK_PAINT_NUMBER_H
#define DESK_PAINT_NUMBER_H

#include "canvas.h"
#include "desk_fonts.h"
#include "desk_model.h"

void desk_paint_number(struct canvas *c, const struct desk_control *ctl,
                       const struct desk_placement *p, const struct desk_fonts *fonts,
                       enum desk_link link);

#endif
