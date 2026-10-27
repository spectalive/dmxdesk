// The dial's echo, logged with its clock beside the frame that asked for it.
#ifndef DESK_APPLY_SPEED_ECHO_H
#define DESK_APPLY_SPEED_ECHO_H

#include "desk_speed.h"

#include <stdint.h>

void desk_apply_speed_echo(struct desk_speed *speed, int widget_id, int ms, int factor, int64_t now);

#endif
