// A command as one frame on the session; -1 when it cannot be encoded or
// the link is not ready.
#ifndef QLC_CONSOLE_SEND_H
#define QLC_CONSOLE_SEND_H

#include "desk_command.h"

int qlc_console_send(void *ctx, const struct desk_command *cmd);

#endif
