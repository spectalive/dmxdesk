#include "osc_msg_make.h"

#include <stdio.h>
#include <string.h>

int osc_msg_make(struct osc_msg *out, const char *address, char type, int i, float f) {
    if (!address[0])
        return -1;
    memset(out, 0, sizeof *out);
    snprintf(out->address, sizeof out->address, "%s", address);
    out->type = type;
    out->i = i;
    out->f = f;
    return 1;
}
