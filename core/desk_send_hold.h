// A hold's frame: 255 on contact, 0 on release, cap or cancel; logged with
// its clock so a stuck output can be read off the log.
// Bursts sit in the hold model under a namespace of their own, so a
// function id never collides with a Flash button's widget id.
#ifndef DESK_SEND_HOLD_H
#define DESK_SEND_HOLD_H

#include "desk_hold.h"
#include "desk_console.h"

#include <stdint.h>

void desk_send_hold(struct desk_hold *hold, const struct desk_console *console, struct hold_action ha,
                    int64_t now);

#endif
