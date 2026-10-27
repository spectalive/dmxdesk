#include "desk_view_family.h"

#include "desk_layout.h"

struct desk_rect desk_view_family(int slot, int families) {
    struct desk_rect none = {0};
    if (families <= 0 || slot < 0 || slot >= families)
        return none;
    int room = DESK_CONTENT_END - DESK_CONTENT_Y - (families - 1) * DESK_GAP;
    int h = room / families < DESK_FAMILY_H ? room / families : DESK_FAMILY_H;
    struct desk_rect r = { DESK_FAMILY_X, DESK_CONTENT_Y + slot * (h + DESK_GAP), DESK_FAMILY_W, h };
    return r;
}
