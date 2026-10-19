#include "desk_show_panel.h"

#include <stdio.h>

int desk_show_panel(struct desk_layout *out, int page, int x, int y, int w, int h,
                    const char *text, int section, enum desk_panel kind) {
    if (out->headings >= DESK_MAX_HEADINGS)
        return -1;
    struct desk_heading *hd = &out->heading[out->headings++];
    *hd = (struct desk_heading){ .page = page, .x = x, .y = y, .w = w, .h = h,
                                 .section = section, .part = 1, .parts = 1, .kind = kind };
    snprintf(hd->text, sizeof hd->text, "%s", text);
    return 0;
}
