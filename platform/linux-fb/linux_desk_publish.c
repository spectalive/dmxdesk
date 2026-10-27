#include "linux_desk_publish.h"

#include <stdio.h>
#include <stdlib.h>

#include "desk_view_snapshot.h"

void linux_desk_publish(struct linux_desk *d, struct desk_app *app, int damaged, int64_t now) {
    if (damaged)
        d->snapshot_pending = 1;
    if (!(d->snapshot_pending && now - d->last_snapshot_ms >= 50) &&
        now - d->last_snapshot_ms < 500)
        return;
    char *view = desk_view_snapshot(&app->model, &app->speed, &app->setup);
    if (view) {
        printf("@STATE %s\n", view);
        fflush(stdout);
        free(view);
        d->snapshot_pending = 0;
    }
    d->last_snapshot_ms = now;
}
