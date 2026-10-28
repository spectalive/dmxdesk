// desk_console.describe for OSC: the command's messages as "/address number",
// separated by "; ".
#ifndef OSC_CONSOLE_DESCRIBE_H
#define OSC_CONSOLE_DESCRIBE_H

#include <stddef.h>

#include "desk_command.h"

int osc_console_describe(void *ctx, const struct desk_command *cmd, char *buf, size_t cap);

#endif
