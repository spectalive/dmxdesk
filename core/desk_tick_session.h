// The master's turn: its sockets, a fresh console, the master's pushes,
// the link on the banner and the speed model's clock.
#ifndef DESK_TICK_SESSION_H
#define DESK_TICK_SESSION_H

#include <stdint.h>

#include "desk_app.h"

void desk_tick_session(struct desk_app *app, int64_t now);

#endif
