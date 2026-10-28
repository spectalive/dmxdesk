// Whether the console reports back the state a command changes: the
// control's own "feedback", the grand master's for its level, and the
// console's for a function or a control the map does not know.
#ifndef OSC_COMMAND_REPORTED_H
#define OSC_COMMAND_REPORTED_H

#include "desk_command.h"
#include "osc_map.h"

int osc_command_reported(const struct osc_map *map, const struct desk_command *cmd);

#endif
