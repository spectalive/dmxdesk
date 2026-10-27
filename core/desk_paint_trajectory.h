// A heads' pick whose caption names a figure (desk_trajectory_of): the
// figure drawn large over its name, the name on one or two lines and never
// cut. Running, the selected look.
#ifndef DESK_PAINT_TRAJECTORY_H
#define DESK_PAINT_TRAJECTORY_H

#include "canvas.h"
#include "desk_fonts.h"
#include "desk_model.h"

void desk_paint_trajectory(struct canvas *c, const struct desk_control *ctl,
                           const struct desk_placement *p, const struct desk_fonts *fonts,
                           enum desk_link link);

#endif
