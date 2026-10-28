#include "qlc_event_of.h"

struct desk_event qlc_event_of(const struct qlc_msg *msg) {
    struct desk_event e = { DESK_EVENT_NONE, -1, 0, 0, 0 };
    switch (msg->kind) {
    case QLC_FUNCTION:
        e.kind = DESK_EVENT_FUNCTION;
        e.id = msg->function_id;
        e.running = msg->running;
        break;
    case QLC_GRAND_MASTER:
        e.kind = DESK_EVENT_GRAND_MASTER;
        e.value = msg->value;
        break;
    case QLC_SPEED_STATE:
        e.kind = DESK_EVENT_SPEED;
        e.id = msg->widget_id;
        e.value = msg->value;
        e.factor = msg->factor;
        break;
    default:
        break;
    }
    return e;
}
