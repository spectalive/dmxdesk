// The next frame the master pushed, decoded; a frame that does not decode
// is an event the desk ignores.
#ifndef QLC_CONSOLE_RECV_H
#define QLC_CONSOLE_RECV_H

#include "desk_event.h"

int qlc_console_recv(void *ctx, struct desk_event *out);

#endif
