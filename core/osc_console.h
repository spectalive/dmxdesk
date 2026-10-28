// A console reached over plain OSC as the desk's console: the link behind
// the desk's console table. The link stays the caller's to create and free.
#ifndef OSC_CONSOLE_H
#define OSC_CONSOLE_H

#include "desk_console.h"
#include "osc_link.h"

struct desk_console osc_console(struct osc_link *link);

#endif
