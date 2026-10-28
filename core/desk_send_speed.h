// A speed frame, logged with its clock so touch-to-echo can be read off the
// log: the tap's down edge is the frame's time, the echo is the push's.
#ifndef DESK_SEND_SPEED_H
#define DESK_SEND_SPEED_H

#include "desk_speed.h"
#include "desk_console.h"

#include <stdint.h>

void desk_send_speed(const struct desk_console *console, struct speed_action action, int64_t now);

#endif
