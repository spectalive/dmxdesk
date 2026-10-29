#include "desk_placement_target.h"

#define ICON_TARGET 44

struct desk_rect desk_placement_target(const struct desk_placement *p) {
    struct desk_rect r = { p->x, p->y, p->w, p->h };
    if (p->tile != TILE_ICON)
        return r;
    if (r.w < ICON_TARGET) {
        r.x -= (ICON_TARGET - r.w) / 2;
        r.w = ICON_TARGET;
    }
    if (r.h < ICON_TARGET) {
        r.y -= (ICON_TARGET - r.h) / 2;
        r.h = ICON_TARGET;
    }
    return r;
}
