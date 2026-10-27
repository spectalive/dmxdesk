#include "desk_paint_tabs.h"

#include "canvas_blend.h"
#include "desk_layout.h"
#include "desk_paint_selected.h"
#include "desk_paint_selected_mark.h"
#include "desk_paint_text_centred.h"
#include "desk_tab.h"
#include "desk_tab_page.h"
#include "desk_tab_title.h"
#include "desk_view.h"

void desk_paint_tabs(struct canvas *c, const struct desk_model *model,
                     const struct desk_fonts *fonts) {
    int on_screen = model->layout.pages > 0 ? model->layout.tab[model->page] : -1;
    for (int i = 0; i < DESK_TABS; i++) {
        if (desk_tab_page(model, i) < 0)
            continue;
        struct desk_rect t = desk_view_tab(i);
        int current = i == on_screen && !model->setup_open;
        // The tab on screen wears the selected look, the others a plain tile.
        if (current) {
            desk_paint_selected(c, t.x, t.y + DESK_TAB_INSET, t.w, t.h - 2 * DESK_TAB_INSET, 10);
            desk_paint_selected_mark(c, t.x, t.y + DESK_TAB_INSET, t.w, t.h - 2 * DESK_TAB_INSET);
        } else {
            canvas_blend_round_rect(c, t.x, t.y + DESK_TAB_INSET, t.w, t.h - 2 * DESK_TAB_INSET, 10, DESK_TILE);
        }
        desk_paint_text_centred(c, fonts->tab, t.x, t.y, t.w, t.h, desk_tab_title((enum desk_tab)i),
                                current ? DESK_ACCENT_INK : DESK_MUTED);
    }
}
