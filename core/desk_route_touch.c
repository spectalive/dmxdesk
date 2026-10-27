#include "desk_route_touch.h"

#include "desk_touch_content.h"
#include "desk_touch_gear.h"
#include "desk_touch_lock.h"
#include "desk_touch_navigate.h"
#include "desk_touch_owned.h"
#include "desk_touch_press.h"
#include "desk_touch_setup.h"
#include "desk_touch_speed_card.h"

void desk_route_touch(struct desk_app *app, const struct desk_platform *p,
                      const struct touch_event *e, int64_t now) {
    if (desk_touch_gear(app, p, e, now) || desk_touch_setup(app, p, e, now) ||
        desk_touch_speed_card(app, p, e, now) || desk_touch_owned(app, e, now) ||
        desk_touch_press(app, p, e, now))
        return;
    int index;
    enum desk_target target = desk_input_feed(&app->input, &app->model, e, &index);
    if (target == TARGET_LOCK) {
        desk_touch_lock(app, e, now);
        return;
    }
    if (!desk_lock_allows(&app->lock))
        return;
    if (target == TARGET_CONTENT)
        desk_touch_content(app, p, e, now);
    else
        desk_touch_navigate(&app->model, target, index);
}
