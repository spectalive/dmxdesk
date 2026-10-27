#include "desk_paint_frame.h"

#include "desk_paint.h"
#include "desk_setup_paint.h"
#include "desk_speed_paint.h"
#include "showmap_validate.h"

void desk_paint_frame(struct desk_app *app, int dx, int dy, int dw, int dh) {
    struct canvas *canvas = &app->canvas;
    struct desk_model *model = &app->model;
    if (dw > 0)
        canvas_set_clip(canvas, dx, dy, dw, dh);
    desk_paint(canvas, model, &app->fonts);
    // SHOW's tempo card: the numbers and the TAP target are the speed model's.
    int ti = DESK_TEMPO_INDEX(&app->map);
    const struct desk_placement *tp = ti < model->count ? desk_placement_of(model, ti) : NULL;
    if (tp && model->control[ti].enabled)
        desk_speed_paint_tempo(canvas, &app->speed, 0, &app->fonts, tp->x, tp->y, tp->w, tp->h,
                               model->control[ti].pressed);
    if (app->layout.speed_page >= 0 && model->page == app->layout.speed_page)
        desk_speed_paint(canvas, &app->speed, &app->fonts);
    desk_paint_overlays(canvas, model, &app->fonts);
    desk_setup_paint(canvas, &app->setup, &app->fonts);
    canvas_clear_clip(canvas);
}
