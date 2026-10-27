#include "desk_end_gestures.h"

#include "desk_release_holds.h"

void desk_end_gestures(struct desk_app *app, int64_t now) {
    desk_cancel_all(&app->model);
    desk_input_cancel_all(&app->input);
    desk_release_holds(&app->hold, &app->model, app->session, app->hold_owner, now);
    desk_speed_touch_cancel(&app->speed);
    desk_speed_reset_taps(&app->speed);
    app->speed_slot = -1;
}
