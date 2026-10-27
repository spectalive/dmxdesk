// The turn's frame on Linux: @STATE on the Qt pipe or a flip on the panel,
// then the periodic report. Returns nonzero when the desk must stop.
#ifndef LINUX_DESK_FRAME_H
#define LINUX_DESK_FRAME_H

#include <stdint.h>

#include "desk_app.h"

int linux_desk_frame(void *ctx, struct desk_app *app, int damaged, int dx, int dy, int dw, int dh,
                     int64_t now);

#endif
