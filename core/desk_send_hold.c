#include "desk_send_hold.h"

#include "desk_burst_base.h"
#include "desk_try_send.h"

#include <stdio.h>

void desk_send_hold(struct desk_hold *hold, const struct desk_console *console, struct hold_action ha,
                    int64_t now) {
    if (ha.widget_id < 0)
        return;
    struct desk_command cmd = ha.widget_id >= BURST_BASE
        ? (struct desk_command){ DESK_CMD_FUNCTION, ha.widget_id - BURST_BASE, ha.on }
        : (struct desk_command){ DESK_CMD_FLASH, ha.widget_id, ha.on };
    char text[64];
    if (desk_try_send(console, &cmd, text, sizeof text) == 0) {
        fprintf(stderr, "hold: sent %s at %lld\n", text, (long long)now);
        return;
    }
    fprintf(stderr, "hold: could not send %s at %lld\n", text, (long long)now);
    // A release that did not go out is still owed: the output may be on.
    if (!ha.on)
        desk_hold_unsent(hold, ha.widget_id);
}
