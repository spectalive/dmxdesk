#include "desk_surface_find.h"

#include <stddef.h>

const struct desk_surface_item *desk_surface_find(const struct desk_surface *s, int id) {
    int lo = 0, hi = s->count - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (s->item[mid].id == id)
            return &s->item[mid];
        if (s->item[mid].id < id)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return NULL;
}
