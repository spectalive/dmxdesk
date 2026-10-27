#include "desk_touch_navigate.h"

#include "desk_tab_page.h"

void desk_touch_navigate(struct desk_model *model, enum desk_target target, int index) {
    if (target == TARGET_RAIL) {
        if (index >= 0 && desk_tab_page(model, index) >= 0)
            desk_set_view(model, desk_tab_page(model, index), -1);
    } else if (target == TARGET_FAMILY) {
        if (index >= 0 && index < model->layout.families)
            desk_set_view(model, model->layout.family_page[index], -1);
    } else if (target == TARGET_BANK) {
        if (index >= 0)
            desk_set_view(model, model->page, index);
    }
}
