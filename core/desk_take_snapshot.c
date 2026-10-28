#include "desk_take_snapshot.h"

#include <stdio.h>

#include "desk_build_holds.h"
#include "desk_surface_free.h"
#include "desk_rebuild_model.h"
#include "desk_release_holds.h"
#include "showmap_validate.h"

void desk_take_snapshot(struct desk_app *app, int64_t now) {
    struct desk_surface fresh;
    if (!app->master.take_snapshot(app->master.ctx, &fresh))
        return;
    desk_surface_free(&app->console);
    app->console = fresh;
    desk_release_holds(&app->hold, &app->model, &app->master, app->hold_owner, now);
    int enabled = desk_rebuild_model(&app->model, &app->map, &app->console, &app->layout);
    desk_build_holds(&app->hold, &app->model, app->hold_owner);
    if (showmap_mismatch(&app->map, &app->console))
        desk_speed_disable(&app->speed, "show mismatch");
    else
        desk_speed_validate(&app->speed, &app->console);
    printf("desk: %d of %d controls enabled against the master's console\n",
           enabled, app->map.count);
}
