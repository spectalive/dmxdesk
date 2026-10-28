#include "desk_surface_copy.h"

#include <stdlib.h>
#include <string.h>

int desk_surface_copy(const struct desk_surface *src, struct desk_surface *out) {
    *out = *src;
    out->item = calloc((size_t)(src->count ? src->count : 1), sizeof *out->item);
    if (!out->item) {
        memset(out, 0, sizeof *out);
        return -1;
    }
    if (src->count)
        memcpy(out->item, src->item, (size_t)src->count * sizeof *out->item);
    return 0;
}
