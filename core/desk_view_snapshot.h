#ifndef DESK_VIEW_SNAPSHOT_H
#define DESK_VIEW_SNAPSHOT_H

#include "desk_model.h"
#include "desk_setup.h"
#include "desk_speed.h"

// Caller frees the returned bounded JSON string. No password field is emitted.
char *desk_view_snapshot(const struct desk_model *model,
                         const struct desk_speed *speed,
                         const struct desk_setup *setup);

#endif
