// The subnet sweep in a child, its output in a file the loop reads when the
// worker exits. The shell gets the addresses as arguments, never in its text.
#ifndef DESK_FINDER_START_H
#define DESK_FINDER_START_H

#include "action_worker.h"

int desk_finder_start(struct action_worker *w, const char *exe, const char *addr, int port);

#endif
