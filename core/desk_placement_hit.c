#include "desk_placement_hit.h"

#include "desk_placement_target.h"

int desk_placement_hit(const struct desk_placement *p, int x, int y) {
    if (p->w <= 0 || p->h <= 0)
        return 0;
    struct desk_rect r = desk_placement_target(p);
    return x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h;
}
