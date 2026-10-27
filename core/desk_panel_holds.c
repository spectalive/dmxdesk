#include "desk_panel_holds.h"

int desk_panel_holds(const struct desk_heading *h, const struct desk_placement *p) {
    return p->page == h->page && p->bank == h->bank && p->x >= h->x && p->x < h->x + h->w &&
           p->y >= h->y && p->y < h->y + h->h;
}
