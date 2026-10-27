#include "linux_desk_finder.h"

#include <stdio.h>

#include "desk_setup_read_found.h"
#include "linux_desk_paths.h"

void linux_desk_finder(struct linux_desk *d, struct desk_app *app) {
    struct desk_setup *setup = &app->setup;
    int exit_status = 0;
    if (!setup->master_busy[0] || !aw_poll(d->finder, &exit_status))
        return;
    FILE *f = fopen(FIND_OUT, "r");
    desk_setup_read_found(setup, f, exit_status);
    if (f)
        fclose(f);
    fprintf(stderr, "desk: find: %d master%s%s\n", setup->found_count,
            setup->found_count == 1 ? "" : "s", setup->found_partial ? " (partial)" : "");
}
