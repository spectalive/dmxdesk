// DMXDESK_DRAG: the master's requested level moves every turn and its tile
// repaints, sending nothing - the load of a finger dragging a fader all
// night, for the perf gate, on a kernel with no uinput to fake a finger.
#ifndef DESK_FAKE_DRAG_H
#define DESK_FAKE_DRAG_H

#include <stdint.h>

#include "desk_model.h"

void desk_fake_drag(struct desk_model *model, int64_t now);

#endif
