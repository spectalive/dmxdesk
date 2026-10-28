// The message a console with no feedback would have sent back for a
// command, so the desk sees what it asked for: a toggle's new state, a
// flash's or a function's 1 or 0, the grand master's level, a dial's time
// or multiplier. 1 and the message, or 0 when there is nothing to report,
// as for a stop-all, whose effect the map cannot know.
#ifndef OSC_ECHO_OF_H
#define OSC_ECHO_OF_H

#include "desk_command.h"
#include "osc_map.h"
#include "osc_msg.h"

int osc_echo_of(const struct osc_map *map, const struct desk_command *cmd, struct osc_msg *out);

#endif
