#include "status_wifi_bars.h"

int status_wifi_bars(const struct status *st) {
    if (!st->have_wifi) return 0;
    return st->level >= -55 ? 3 : st->level >= -70 ? 2 : 1;
}
