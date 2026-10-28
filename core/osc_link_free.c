#include "osc_link_free.h"

#include <stdlib.h>

#include "osc_link_close.h"
#include "osc_map_free.h"

void osc_link_free(struct osc_link *l) {
    if (!l)
        return;
    osc_link_close(l);
    osc_map_free(&l->map);
    free(l);
}
