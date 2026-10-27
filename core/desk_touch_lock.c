#include "desk_touch_lock.h"

#include <stdio.h>

#include "desk_end_gestures.h"

void desk_touch_lock(struct desk_app *app, const struct touch_event *e, int64_t now) {
    int changed = e->kind == TOUCH_DOWN ? desk_lock_target_down(&app->lock, now)
                : e->kind == TOUCH_UP ? desk_lock_target_up(&app->lock, now) : 0;
    if (!changed)
        return;
    desk_end_gestures(app, now);
    fprintf(stderr, "desk: surface %s\n", app->lock.state == DESK_LOCKED ? "locked" : "unlocked");
}
