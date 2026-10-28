#include "desk_send_command.h"

#include "desk_action_log.h"

void desk_send_command(const struct desk_console *console, const char *tag,
                       const struct desk_command *cmd, int64_t now) {
    char text[64] = "";
    console->describe(console->ctx, cmd, text, sizeof text);
    if (console->send(console->ctx, cmd) == 0)
        desk_action_log_sent(tag, text, now);
    else
        desk_action_log_dropped(tag, text, "link down");
}
