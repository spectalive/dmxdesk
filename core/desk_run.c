#include "desk_run.h"

#include "desk_damage_speed.h"
#include "desk_fake_drag.h"
#include "desk_log_link.h"
#include "desk_log_rtt.h"
#include "desk_sync_link_word.h"
#include "desk_take_frame_damage.h"
#include "desk_take_power_key.h"
#include "desk_take_touches.h"
#include "desk_tick_holds.h"
#include "desk_tick_session.h"
#include "desk_tick_status.h"
#include "desk_toggle_drain.h"
#include "loop_trace.h"

void desk_run(struct desk_app *app, const struct desk_platform *p) {
    while (!app->stop && !p->stopping(p->ctx)) {
        int64_t iteration_at = loop_trace(NULL, 0);
        int64_t phase_at = iteration_at;
        struct pollfd fds[2];
        int count = app->master.pollfds(app->master.ctx, fds, 2);
        p->wait(p->ctx, fds, count, app->no_wait ? 0 : 100);
        phase_at = loop_trace("poll", phase_at);
        int64_t now = p->now(p->ctx);
        if (app->fake_drag)
            desk_fake_drag(&app->model, now);
        if (p->power_key(p->ctx))
            desk_take_power_key(app, p, now);
        phase_at = loop_trace("power-input", phase_at);
        desk_take_touches(app, p, now);
        // A toggle held in the spacing queue since an earlier turn, whose
        // slot has now come.
        desk_toggle_drain(&app->master, &app->toggles, now);
        phase_at = loop_trace("touch-input", phase_at);
        p->service(p->ctx, app, now);
        phase_at = loop_trace("services", phase_at);
        desk_tick_session(app, now);
        phase_at = loop_trace("qlc-step", phase_at);
        desk_tick_holds(app, now);
        desk_damage_speed(app);
        desk_set_locked(&app->model, app->lock.state != DESK_UNLOCKED);
        desk_log_rtt(app);
        desk_log_link(app, now);
        desk_sync_link_word(app);
        phase_at = loop_trace("qlc-frames-model", phase_at);
        desk_tick_status(app, p, now);
        phase_at = loop_trace("status", phase_at);
        int dx = 0, dy = 0, dw = -1, dh = -1;
        int damaged = desk_take_frame_damage(app, &dx, &dy, &dw, &dh);
        if (p->frame(p->ctx, app, damaged, dx, dy, dw, dh, now))
            break;
        loop_trace("frame", phase_at);
        loop_trace("iteration", iteration_at);
    }
}
