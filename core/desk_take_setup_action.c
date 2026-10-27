#include "desk_take_setup_action.h"

#include <stdio.h>

void desk_take_setup_action(struct desk_app *app, const struct desk_platform *p,
                            const struct setup_action *act, int64_t now) {
    switch (act->kind) {
    case SETUP_NONE:
        break;
    case SETUP_CLOSE:
        desk_setup_close(&app->setup);
        app->bar_ready = 0;
        app->last_status_ms = 0;
        break;
    case SETUP_SET_MASTER:
        snprintf(app->conf.master, sizeof app->conf.master, "%s", act->host);
        app->conf.port = act->port;
        if (desk_conf_save(&app->conf, app->conf_path) != 0) {
            snprintf(app->setup.save_note, sizeof app->setup.save_note, "Applied, not saved");
            fprintf(stderr, "desk: cannot save %s\n", app->conf_path);
        } else {
            app->setup.save_note[0] = '\0';
        }
        qlc_session_set_host(app->session, app->conf.master, app->conf.port);
        desk_setup_set_master(&app->setup, app->conf.master, app->conf.port, 1);
        fprintf(stderr, "desk: master %s:%d\n", app->conf.master, app->conf.port);
        break;
    default:
        p->setup_action(p->ctx, app, act, now);
        break;
    }
}
