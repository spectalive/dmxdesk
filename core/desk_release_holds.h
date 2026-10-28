// Every hold released at once: a lock, a page change, the surface opening,
// a blank. The hold owners are forgotten and the tiles repainted idle.
#ifndef DESK_RELEASE_HOLDS_H
#define DESK_RELEASE_HOLDS_H

#include "desk_hold.h"
#include "desk_model.h"
#include "desk_console.h"

#include <stdint.h>

void desk_release_holds(struct desk_hold *hold, struct desk_model *model, const struct desk_console *console,
                        int *owner, int64_t now);

#endif
