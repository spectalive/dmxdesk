#include "osc_factor_key.h"

#include <stdlib.h>

int osc_factor_key(const char *key) {
    if (!key || !key[0] || key[0] < '0' || key[0] > '9')
        return -1;
    char *end;
    long v = strtol(key, &end, 10);
    return *end || v > 10 ? -1 : (int)v;
}
