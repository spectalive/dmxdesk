// For a console without feedback, loops back the reply it would have sent
// for a command it has just been sent.
#ifndef OSC_LINK_ECHO_H
#define OSC_LINK_ECHO_H

#include "desk_command.h"
#include "osc_link.h"

void osc_link_echo(struct osc_link *l, const struct desk_command *cmd);

#endif
