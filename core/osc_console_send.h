// desk_console.send for OSC: the command's messages to the console, then the
// reply a console without feedback would have sent. 0 sent, -1 dropped.
#ifndef OSC_CONSOLE_SEND_H
#define OSC_CONSOLE_SEND_H

#include "desk_command.h"

int osc_console_send(void *ctx, const struct desk_command *cmd);

#endif
