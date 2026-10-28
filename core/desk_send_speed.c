#include "desk_send_speed.h"

#include <stdio.h>

#include "desk_try_send.h"

void desk_send_speed(const struct desk_console *console, struct speed_action action, int64_t now) {
    struct desk_command cmd;
    switch (action.kind) {
    case SPEED_ACT_TIME:
    case SPEED_ACT_TIME_BOTH:
        cmd = (struct desk_command){ DESK_CMD_SPEED_MS, action.widget_id, action.ms };
        break;
    case SPEED_ACT_FACTOR:
        cmd = (struct desk_command){ DESK_CMD_SPEED_FACTOR, action.widget_id, action.factor };
        break;
    case SPEED_ACT_NONE:
    default:
        return;
    }
    char text[64];
    if (desk_try_send(console, &cmd, text, sizeof text) == 0)
        fprintf(stderr, "speed: sent %s at %lld\n", text, (long long)now);
    if (action.kind != SPEED_ACT_TIME_BOTH || action.widget_id2 < 0)
        return;
    cmd = (struct desk_command){ DESK_CMD_SPEED_MS, action.widget_id2, action.ms2 };
    if (desk_try_send(console, &cmd, text, sizeof text) == 0)
        fprintf(stderr, "speed: sent %s at %lld\n", text, (long long)now);
}
