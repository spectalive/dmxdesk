// desk_console.step for OSC: opens the socket when it is closed and its retry
// is due. READY while it is open, DOWN otherwise; a fresh open offers the
// surface as a new snapshot.
#ifndef OSC_CONSOLE_STEP_H
#define OSC_CONSOLE_STEP_H

#include <stdint.h>

#include "desk_console_link.h"

enum desk_console_link osc_console_step(void *ctx, int64_t now);

#endif
