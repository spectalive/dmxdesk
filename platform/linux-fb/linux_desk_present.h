// The panel's frame: a damaged frame paints only its rectangle, and a forced
// flip re-presents the same canvas - the point is the flip, not the paint,
// and a full repaint on this CPU would cap the rate far below the panel's.
// A dump waits for the link, so the frame shows the show and not the banner;
// six seconds without one and it shows that instead. Returns nonzero when
// the desk must stop: the flip failed, or the dump is written.
#ifndef LINUX_DESK_PRESENT_H
#define LINUX_DESK_PRESENT_H

#include <stdint.h>

#include "desk_app.h"
#include "linux_desk.h"

int linux_desk_present(struct linux_desk *d, struct desk_app *app, int damaged,
                       int dx, int dy, int dw, int dh, int64_t now);

#endif
