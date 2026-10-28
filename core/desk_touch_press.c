#include "desk_touch_press.h"

#include "desk_send_hold.h"
#include "desk_send_speed.h"
#include "desk_speed_paint.h"

int desk_touch_press(struct desk_app *app, const struct desk_platform *p,
                     const struct touch_event *e, int64_t now) {
    int slot = e->slot, x = (int)e->x, y = (int)e->y;
    if (e->kind != TOUCH_DOWN || slot < 0 || slot >= TOUCH_MAX_SLOTS || !desk_lock_allows(&app->lock))
        return 0;
    struct desk_model *model = &app->model;
    int ci = desk_control_at(model, x, y);
    if (ci >= 0 && (model->control[ci].kind == DESK_HOLD || model->control[ci].kind == DESK_BURST)) {
        struct desk_control *hc = &model->control[ci];
        if (hc->enabled && hc->hold_index >= 0 && model->link == DESK_LINK_READY) {
            struct hold_action ha = desk_hold_press(&app->hold, hc->hold_index, slot, now);
            if (ha.widget_id >= 0) {
                desk_send_hold(&app->hold, &app->master, ha, now);
                app->hold_owner[slot] = ci;
                desk_set_hold_progress(model, ci, 0, 1);
            }
        }
        return 1;
    }
    if (ci < 0 || model->control[ci].kind != DESK_TEMPO || !model->control[ci].enabled)
        return 0;
    const struct desk_placement *tp = desk_placement_of(model, ci);
    if (!tp)
        return 1;
    int tx, ty, tw, th;
    desk_speed_tempo_tap_rect(tp->x, tp->y, tp->w, tp->h, &tx, &ty, &tw, &th);
    if (x >= tx && x < tx + tw && y >= ty && y < ty + th) {
        int64_t at = e->t > 0 ? (int64_t)(e->t * 1000.0) : now;
        desk_send_speed(&app->master, desk_speed_direct(&app->speed, 0, SPEED_T_TAP,
                                                        p->now(p->ctx), at), now);
        app->hold_owner[slot] = ci;     // the release is consumed, the target reads pressed
        desk_set_hold_progress(model, ci, -1, 1);
        return 1;
    }
    for (int step = SPEED_T_BPM_DOWN; step <= SPEED_T_BPM_UP; step++) {
        desk_speed_tempo_step_rect(tp->x, tp->y, tp->w, tp->h,
                                   (enum speed_target)step, &tx, &ty, &tw, &th);
        if (x < tx || x >= tx + tw || y < ty || y >= ty + th)
            continue;
        desk_send_speed(&app->master, desk_speed_direct(&app->speed, 0, (enum speed_target)step,
                                                        p->now(p->ctx), now), now);
        app->hold_owner[slot] = ci;
        break;
    }
    return 1;
}
