// A command as the frame QLC+ would receive, for the log.
#ifndef QLC_CONSOLE_DESCRIBE_H
#define QLC_CONSOLE_DESCRIBE_H

#include <stddef.h>

#include "desk_command.h"

int qlc_console_describe(void *ctx, const struct desk_command *cmd, char *buf, size_t cap);

#endif
