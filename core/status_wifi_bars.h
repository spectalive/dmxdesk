// The bar's Wi-Fi strength. iOS lights three bars from about -55 dBm, two to
// about -70, one below.
#ifndef STATUS_WIFI_BARS_H
#define STATUS_WIFI_BARS_H

#include "status.h"

int status_wifi_bars(const struct status *st);

#endif
