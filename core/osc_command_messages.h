// A desk command as the OSC messages that carry it, into out[0..1]. A press
// is a button going to 1 and back to 0, which is how a console reads a
// momentary OSC input; a flash and a function carry their 1 or 0; the grand
// master its level as 0..1; a dial its time as an int in ms, or as 0..1 of
// its msRange, and its multiplier as an int on its factorAddress or as a
// press of that multiplier's button. How many messages, or -1 for a command
// the map cannot address.
#ifndef OSC_COMMAND_MESSAGES_H
#define OSC_COMMAND_MESSAGES_H

#include "desk_command.h"
#include "osc_map.h"
#include "osc_msg.h"

int osc_command_messages(const struct osc_map *map, const struct desk_command *cmd,
                         struct osc_msg out[2]);

#endif
