#include "osc_map_load.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "osc_map_parse.h"

#define OSC_MAP_MAX_BYTES (512 * 1024)

int osc_map_load(const char *path, struct osc_map *out, char *why, size_t cap) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        snprintf(why, cap, "%s", strerror(errno));
        return -1;
    }
    char *buf = malloc(OSC_MAP_MAX_BYTES);
    size_t len = buf ? fread(buf, 1, OSC_MAP_MAX_BYTES, f) : 0;
    int too_big = buf && len == OSC_MAP_MAX_BYTES;
    fclose(f);
    int rc = -1;
    if (!buf)
        snprintf(why, cap, "out of memory");
    else if (too_big)
        snprintf(why, cap, "larger than %d bytes", OSC_MAP_MAX_BYTES);
    else
        rc = osc_map_parse(buf, len, out, why, cap);
    free(buf);
    return rc;
}
