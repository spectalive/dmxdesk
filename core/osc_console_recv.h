// desk_console.recv for OSC: the next message from the console, looped-back
// replies first, applied to the map and read as a desk event.
#ifndef OSC_CONSOLE_RECV_H
#define OSC_CONSOLE_RECV_H

#include "desk_event.h"

int osc_console_recv(void *ctx, struct desk_event *out);

#endif
