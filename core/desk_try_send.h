// One command to the console, its wire text left in text for the caller's
// log. Returns 0 when it went out, -1 when it could not be expressed or the
// link dropped it.
#ifndef DESK_TRY_SEND_H
#define DESK_TRY_SEND_H

#include <stddef.h>

#include "desk_command.h"
#include "desk_console.h"

int desk_try_send(const struct desk_console *console, const struct desk_command *cmd,
                  char *text, size_t cap);

#endif
