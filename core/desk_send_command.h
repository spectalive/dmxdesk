// One command to the console, logged under a tag with the text the console's
// wire carries: "sent" when it went out, "dropped" when the link was down.
#ifndef DESK_SEND_COMMAND_H
#define DESK_SEND_COMMAND_H

#include <stdint.h>

#include "desk_command.h"
#include "desk_console.h"

void desk_send_command(const struct desk_console *console, const char *tag,
                       const struct desk_command *cmd, int64_t now);

#endif
