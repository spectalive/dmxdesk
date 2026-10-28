// A retry every second and a half is one state to the operator, not a
// banner flipping between two words; the link is down only without a master.
#ifndef DESK_LINK_OF_H
#define DESK_LINK_OF_H

#include "desk_model.h"
#include "desk_console_link.h"

enum desk_link desk_link_of(enum desk_console_link link, int have_master);

#endif
