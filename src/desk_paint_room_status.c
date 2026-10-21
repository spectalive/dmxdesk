#include "desk_paint_room_status.h"

#include "desk_layout.h"
#include "desk_paint_text_at.h"
#include "desk_room_summary.h"

void desk_paint_room_status(struct canvas *canvas, const struct desk_model *model,
                            int x, int y, int width, const struct desk_fonts *fonts) {
    const struct desk_room_summary summary = desk_room_summary(model);
    const char *labels[3] = { "EFECTO", "HUMO", "COLOR" };
    const char *values[3] = { summary.effect, summary.smoke, summary.colour };
    for (int i = 0; i < 3; i++) {
        int row = y + i * 26;
        desk_paint_text_at(canvas, fonts->small, x, row, 60, labels[i], DESK_HERO_SOFT);
        desk_paint_text_at(canvas, fonts->tile_s, x + 62, row - 1, width - 62, values[i], DESK_INK);
    }
}
