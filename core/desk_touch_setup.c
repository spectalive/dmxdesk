#include "desk_touch_setup.h"

#include <string.h>

#include "desk_setup_layout.h"
#include "desk_take_setup_action.h"

int desk_touch_setup(struct desk_app *app, const struct desk_platform *p,
                     const struct touch_event *e, int64_t now) {
    int slot = e->slot, x = (int)e->x, y = (int)e->y;
    if (slot < 0 || slot >= TOUCH_MAX_SLOTS)
        return 0;
    if (app->setup.open && e->kind == TOUCH_DOWN && x < SETUP_SHEET_W) {
        if (app->setup_slot >= 0)
            return 1;
        app->setup_slot = slot;
    }
    if (slot != app->setup_slot)
        return 0;
    struct setup_action act;
    memset(&act, 0, sizeof act);
    switch (e->kind) {
    case TOUCH_DOWN: act = desk_setup_touch_down(&app->setup, x, y); break;
    case TOUCH_MOVE: act = desk_setup_touch_move(&app->setup, x, y); break;
    case TOUCH_UP: act = desk_setup_touch_up(&app->setup, x, y); app->setup_slot = -1; break;
    case TOUCH_CANCEL: desk_setup_touch_cancel(&app->setup); app->setup_slot = -1; break;
    }
    desk_take_setup_action(app, p, &act, now);
    memset(&act, 0, sizeof act);
    return 1;
}
