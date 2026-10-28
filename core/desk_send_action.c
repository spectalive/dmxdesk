#include "desk_send_action.h"

#include "desk_action_log.h"
#include "desk_release_hook.h"
#include "desk_send_command.h"

void desk_send_action(const struct desk_console *console, struct desk_toggle_queue *toggles,
                      struct desk_model *model, const struct show_map *map,
                      struct desk_action action, int64_t now) {
    char text[64];
    struct desk_command cmd;
    switch (action.kind) {
    case DESK_ACT_TOGGLE: {
        int hook = desk_release_hook(model, map, action.widget_id);
        cmd = (struct desk_command){ DESK_CMD_TOGGLE, action.widget_id, 0 };
        if (console->describe(console->ctx, &cmd, text, sizeof text) <= 0)
            return;
        if (desk_toggle_queue_push(toggles, &cmd) != 0) {
            desk_action_log_dropped("toggle", text, "queue full");
            return;
        }
        desk_note_sent(model, action.widget_id, now);
        cmd = (struct desk_command){ DESK_CMD_TOGGLE, hook, 0 };
        if (hook < 0 || console->describe(console->ctx, &cmd, text, sizeof text) <= 0)
            return;
        if (desk_toggle_queue_push(toggles, &cmd) != 0) {
            desk_action_log_dropped("toggle", text, "queue full");
            return;
        }
        // Both stay pending until the console answers: a second tap before
        // then is not a second release, and the hook is not pressed off again.
        desk_note_sent(model, hook, now);
        return;
    }
    case DESK_ACT_HAZE_OFF:
        cmd = (struct desk_command){ DESK_CMD_FUNCTION, action.widget_id, 0 };
        if (console->describe(console->ctx, &cmd, text, sizeof text) <= 0)
            return;
        while (desk_toggle_queue_push(toggles, &cmd) != 0) {
            struct desk_command waiting;
            if (!desk_toggle_queue_pop(toggles, &waiting))
                return;
            console->describe(console->ctx, &waiting, text, sizeof text);
            desk_action_log_dropped("toggle", text, "haze off priority");
        }
        return;
    case DESK_ACT_MASTER:
        cmd = (struct desk_command){ DESK_CMD_GRAND_MASTER, 0, action.value };
        console->send(console->ctx, &cmd);
        return;
    case DESK_ACT_STOP_ALL: {
        cmd = (struct desk_command){ DESK_CMD_STOP_ALL, action.widget_id, 0 };
        if (console->describe(console->ctx, &cmd, text, sizeof text) <= 0)
            return;
        // A toggle still waiting for its slot would land after the stop and
        // start something again: stop-all empties the queue first.
        struct desk_command waiting;
        while (desk_toggle_queue_pop(toggles, &waiting)) {
            console->describe(console->ctx, &waiting, text, sizeof text);
            desk_action_log_dropped("toggle", text, "stop-all");
        }
        desk_send_command(console, "stop", &cmd, now);
        return;
    }
    case DESK_ACT_NONE:
        return;
    }
}
