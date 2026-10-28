#include "desk_toggle_drain.h"

#include "desk_send_command.h"

void desk_toggle_drain(const struct desk_console *console, struct desk_toggle_queue *toggles,
                       int64_t now) {
    if (!desk_toggle_queue_slot_open(toggles, now))
        return;
    struct desk_command cmd;
    if (!desk_toggle_queue_pop(toggles, &cmd))
        return;
    desk_toggle_queue_mark_slot(toggles, now);
    desk_send_command(console, "toggle", &cmd, now);
}
