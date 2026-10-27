// The hook a pick's release gives its family back to. In QLC+ 5 a latched
// pick released in a solo frame leaves nothing running in that frame (the
// show's rest scenes keep the family still), so the desk follows the
// release with a press of the hook the room state runs there (2026-09-26
// audit). Pure: the caller queues the press behind the release.
#ifndef DESK_RELEASE_HOOK_H
#define DESK_RELEASE_HOOK_H

#include "desk_model.h"
#include "showmap.h"

// For a toggle of `widget_id` the operator just struck: the widget of the
// hook to press after it, or -1 for none. A hook is pressed only when all of
// these hold: the control is a cue the master says is on (so the tap turns
// it off), its releaseTo has an entry for the state control that is on, and
// that hook is enabled and known to be off (a Toggle pressed while it runs
// stops it, and an unknown one is not pressed blind).
int desk_release_hook(const struct desk_model *model, const struct show_map *map, int widget_id);

#endif
