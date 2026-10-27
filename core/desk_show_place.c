#include "desk_show_place.h"

int desk_show_place(struct desk_layout *out, int control, int page, int x, int y, int w, int h,
                    enum desk_tile tile) {
    if (out->placements >= DESK_MAX_PLACEMENTS)
        return -1;
    struct desk_placement *p = &out->placement[out->placements++];
    *p = (struct desk_placement){ control, page, 0, x, y, w, h, tile };
    return 0;
}
