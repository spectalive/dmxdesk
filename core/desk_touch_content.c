#include "desk_touch_content.h"

#include "desk_master_track.h"
#include "desk_send_action.h"
#include "desk_send_speed.h"

void desk_touch_content(struct desk_app *app, const struct desk_platform *p,
                        const struct touch_event *e, int64_t now) {
    struct desk_model *model = &app->model;
    int slot = e->slot, x = (int)e->x, y = (int)e->y;
    // Resolve against the captured placement while dragging, even when the
    // finger has left the tile. Fonts stay at this boundary.
    int placement = model->capture_placement;
    const struct desk_placement *mp = placement >= 0
        ? &model->layout.placement[placement]
        : desk_placement_of(model, desk_control_at(model, x, y));
    struct desk_rect master_track = desk_master_track(mp, app->fonts.value);
    struct desk_action action = { DESK_ACT_NONE, -1, 0 };
    switch (e->kind) {
    case TOUCH_DOWN:
        action = desk_touch_down(model, slot, x, y, &master_track);
        if (app->layout.speed_page >= 0 && model->page == app->layout.speed_page &&
            model->capture_slot != slot && app->speed_slot < 0) {
            // Tempo is measured from the contact's own clock, not from when
            // the batch was drained.
            int64_t at = e->t > 0 ? (int64_t)(e->t * 1000.0) : now;
            struct speed_action sa = desk_speed_touch_down(&app->speed, x, y, p->now(p->ctx), at);
            // Dead space claims nothing: another finger may still reach a
            // target while this one rests on the card.
            if (app->speed.capture != SPEED_T_NONE)
                app->speed_slot = slot;
            desk_send_speed(app->session, sa, now);
            return;
        }
        break;
    case TOUCH_MOVE:
        action = desk_touch_move(model, slot, x, y, &master_track);
        break;
    case TOUCH_UP:
        action = desk_touch_up(model, slot, x, y);
        break;
    case TOUCH_CANCEL:
        desk_touch_cancel(model, slot);
        break;
    }
    desk_send_action(app->session, &app->toggles, model, &app->map, action, now);
}
