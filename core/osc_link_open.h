// Resolves the console and opens a non-blocking UDP socket of its family,
// bound to the listen port when there is one. 0, or -1 with the reason kept.
#ifndef OSC_LINK_OPEN_H
#define OSC_LINK_OPEN_H

#include "osc_link.h"

int osc_link_open(struct osc_link *l);

#endif
