// Where target `t` of dial `dial`'s card is, shared by the painter, the hit
// test and the inert mask so a target is pressed exactly where it is drawn.
// Tap both has no dial. A target RITMO does not draw is an empty rectangle.
#ifndef DESK_SPEED_TARGET_RECT_H
#define DESK_SPEED_TARGET_RECT_H

#include "desk_speed.h"
#include "desk_view.h"

struct desk_rect desk_speed_target_rect(int dial, enum speed_target t);

#endif
