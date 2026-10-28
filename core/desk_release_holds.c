#include "desk_release_holds.h"

#include "desk_send_hold.h"
#include "touch_event.h"

void desk_release_holds(struct desk_hold *hold, struct desk_model *model, const struct desk_console *console,
                        int *owner, int64_t now) {
    struct hold_action out[MAP_MAX_CONTROLS];
    int n = desk_hold_release_all(hold, now, out, MAP_MAX_CONTROLS);
    for (int k = 0; k < n; k++)
        desk_send_hold(hold, console, out[k], now);
    for (int s = 0; s < TOUCH_MAX_SLOTS; s++) {
        if (owner[s] >= 0)
            desk_set_hold_progress(model, owner[s], -1, 0);
        owner[s] = -1;
    }
}
