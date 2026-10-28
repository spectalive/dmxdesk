// desk_console.take_snapshot for OSC: a copy of the surface the file declared,
// with the state the console has reported since, once per open or refresh.
#ifndef OSC_CONSOLE_TAKE_SNAPSHOT_H
#define OSC_CONSOLE_TAKE_SNAPSHOT_H

#include "desk_surface.h"

int osc_console_take_snapshot(void *ctx, struct desk_surface *out);

#endif
