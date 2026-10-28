// One event from the console onto the models: a function's state, the grand
// master, a speed dial's echo. Events the desk does not act on are ignored.
#ifndef DESK_APPLY_EVENT_H
#define DESK_APPLY_EVENT_H

#include <stdint.h>

#include "desk_event.h"
#include "desk_model.h"
#include "desk_speed.h"

void desk_apply_event(struct desk_model *model, struct desk_speed *speed,
                      const struct desk_event *event, int64_t now);

#endif
