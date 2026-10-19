#include "desk_paint_families.h"

#include "desk_layout.h"
#include "desk_paint_keyline.h"
#include "desk_paint_selected.h"
#include "desk_paint_selected_mark.h"
#include "desk_paint_text_centred.h"
#include "desk_view_family.h"
#include "icon.h"

// The mark 12 px in from the left, the name in what remains: "CONTROL" in
// 14 px capitals is 69 px, and 72 remain.
#define MARK_X 12
#define NAME_X (MARK_X + ICON_SIZE + 6)

void desk_paint_families(struct canvas *c, const struct desk_model *model,
                         const struct desk_fonts *fonts) {
    const struct desk_layout *l = &model->layout;
    for (int i = 0; i < l->families; i++) {
        struct desk_rect r = desk_view_family(i, l->families);
        int current = l->family_page[i] == model->page;
        uint32_t ink = current ? DESK_ACCENT_INK : DESK_INK;
        // The family on screen wears the selected look.
        if (current) {
            desk_paint_selected(c, r.x, r.y, r.w, r.h, DESK_RADIUS);
            desk_paint_selected_mark(c, r.x, r.y, r.w, r.h);
        } else {
            desk_paint_keyline(c, r.x, r.y, r.w, r.h, DESK_RADIUS, DESK_TILE, DESK_LINE);
        }
        int name_x = r.x + 8;
        if (l->family_icon[i] >= 0) {
            icon_paint(c, (enum icon_id)l->family_icon[i], r.x + MARK_X, r.y + (r.h - ICON_SIZE) / 2,
                       ink);
            name_x = r.x + NAME_X;
        }
        // The name starts beside the mark: a box as wide as the name, capped.
        const char *name = l->title[l->family_page[i]];
        int room = r.x + r.w - 8 - name_x;
        int width = fonts->tile_s ? font_width(fonts->tile_s, name) : canvas_text_width(name, 3);
        desk_paint_text_centred(c, fonts->tile_s, name_x, r.y, width < room ? width : room, r.h,
                                name, ink);
    }
}
