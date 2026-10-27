// Reads the battery and the Wi-Fi link from the kernel into a struct status.
#ifndef STATUS_READ_H
#define STATUS_READ_H

#include "status.h"

#include <stdint.h>

void status_read(struct status *st);
void status_power_read_at(struct status *st, const char *root, int64_t read_ms);
// iOS lights three bars from about -55 dBm, two to about -70, one below.
int status_wifi_bars(const struct status *st);

#endif
