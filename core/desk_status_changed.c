#include "desk_status_changed.h"

#include "status_wifi_bars.h"

int desk_status_changed(const struct status *a, const struct status *b) {
    return a->have_batt != b->have_batt || a->cap != b->cap || a->plugged != b->plugged ||
           status_wifi_bars(a) != status_wifi_bars(b);
}
