// A tap on the rail, a family tab or a bank tab: the view moves.
#ifndef DESK_TOUCH_NAVIGATE_H
#define DESK_TOUCH_NAVIGATE_H

#include "desk_input.h"
#include "desk_model.h"

void desk_touch_navigate(struct desk_model *model, enum desk_target target, int index);

#endif
