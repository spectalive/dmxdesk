#include "desk_touch_gear.h"

#include "desk_in_gear.h"
#include "desk_toggle_setup.h"

int desk_touch_gear(struct desk_app *app, const struct desk_platform *p,
                    const struct touch_event *e, int64_t now) {
    int slot = e->slot, x = (int)e->x, y = (int)e->y;
    if (slot < 0 || slot >= TOUCH_MAX_SLOTS)
        return 0;
    if (e->kind == TOUCH_DOWN && desk_in_gear(x, y) && desk_lock_allows(&app->lock)) {
        app->gear_slot = slot;
        return 1;
    }
    if (slot != app->gear_slot)
        return 0;
    if (e->kind == TOUCH_UP || e->kind == TOUCH_CANCEL) {
        app->gear_slot = -1;
        if (e->kind == TOUCH_UP && desk_in_gear(x, y))
            desk_toggle_setup(app, p, now);
    }
    return 1;
}
