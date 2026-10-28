// A desk command as one QLC+ web-access frame. Its length, or -1 when the
// command does not fit the grammar or the buffer.
#ifndef QLC_ENCODE_COMMAND_H
#define QLC_ENCODE_COMMAND_H

#include <stddef.h>

#include "desk_command.h"

int qlc_encode_command(char *buf, size_t cap, const struct desk_command *cmd);

#endif
