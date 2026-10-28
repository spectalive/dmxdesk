#include "desk_surface_free.h"

#include <stdlib.h>
#include <string.h>

void desk_surface_free(struct desk_surface *s) {
    free(s->item);
    memset(s, 0, sizeof *s);
}
