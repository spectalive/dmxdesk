#ifndef DESK_APPLY_FRAME_H
#define DESK_APPLY_FRAME_H

#include "desk_model.h"
#include "desk_speed.h"

#include <stdint.h>

void desk_apply_frame(struct desk_model *model, struct desk_speed *speed, const char *frame,
                      int64_t now);

#endif
