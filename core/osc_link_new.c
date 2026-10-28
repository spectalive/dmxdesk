#include "osc_link_new.h"

#include <stdio.h>
#include <stdlib.h>

#include "osc_map_load.h"

struct osc_link *osc_link_new(const struct osc_link_config *cfg) {
    struct osc_link *l = calloc(1, sizeof *l);
    if (!l)
        return NULL;
    char why[128];
    if (osc_map_load(cfg->surface_path, &l->map, why, sizeof why) != 0) {
        fprintf(stderr, "osc: %s: %s\n", cfg->surface_path, why);
        free(l);
        return NULL;
    }
    snprintf(l->host, sizeof l->host, "%s", cfg->host);
    l->port = cfg->port;
    l->listen_port = cfg->listen_port;
    l->fd = -1;
    snprintf(l->reason, sizeof l->reason, "not opened yet");
    return l;
}
