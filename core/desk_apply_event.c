#include "desk_apply_event.h"

#include "desk_apply_speed_echo.h"

void desk_apply_event(struct desk_model *model, struct desk_speed *speed,
                      const struct desk_event *event, int64_t now) {
    switch (event->kind) {
    case DESK_EVENT_FUNCTION:
        desk_apply_function(model, event->id, event->running);
        break;
    case DESK_EVENT_GRAND_MASTER:
        desk_apply_master(model, event->value);
        break;
    case DESK_EVENT_SPEED:
        desk_apply_speed_echo(speed, event->id, event->value, event->factor, now);
        break;
    case DESK_EVENT_NONE:
        break;
    }
}
