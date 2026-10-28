// One gesture, one command. A toggle is queued so two struck in the same engine
// tick cannot both land before either one's effect is read back (2026-09-26:
// AUTO plus a colour pick of the same solo frame left the pick's colour
// stopped with RGB at 0 while AUTO ran); stop-all and the master go at once.
// A pick released in its solo frame is followed, one slot later, by the hook
// the room state runs there (desk_release_hook), queued like any toggle.
#ifndef DESK_SEND_ACTION_H
#define DESK_SEND_ACTION_H

#include "desk_model.h"
#include "desk_toggle_queue.h"
#include "desk_console.h"
#include "showmap.h"

#include <stdint.h>

void desk_send_action(const struct desk_console *console, struct desk_toggle_queue *toggles,
                      struct desk_model *model, const struct show_map *map,
                      struct desk_action action, int64_t now);

#endif
