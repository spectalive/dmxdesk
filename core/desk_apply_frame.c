#include "desk_apply_frame.h"

#include "desk_apply_speed_echo.h"
#include "qlc_codec.h"

#include <string.h>

void desk_apply_frame(struct desk_model *model, struct desk_speed *speed, const char *frame,
                      int64_t now) {
    struct qlc_msg msg;
    if (qlc_decode(frame, strlen(frame), &msg) != 0)
        return;
    switch (msg.kind) {
    case QLC_FUNCTION:
        desk_apply_function(model, msg.function_id, msg.running);
        break;
    case QLC_GRAND_MASTER:
        desk_apply_master(model, msg.value);
        break;
    case QLC_SPEED_STATE:
        desk_apply_speed_echo(speed, msg.widget_id, msg.value, msg.factor, now);
        break;
    default:
        break;
    }
}
