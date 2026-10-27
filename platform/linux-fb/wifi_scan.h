// The networks a scan found, out of wpa_supplicant's SCAN_RESULTS table:
// one entry per SSID, the strongest of its access points, sorted by signal.
// Hidden networks (an empty SSID) are dropped; the security the flags
// declare decides whether the desk can offer to join.
#ifndef WIFI_SCAN_H
#define WIFI_SCAN_H

#include "wifi_network.h"

#include <stddef.h>

// Parses the table (`bssid / frequency / signal level / flags / ssid`, a
// header line, tab-separated). Never fails on a malformed line: it is
// skipped. Returns the count.
int wifi_scan_parse(const char *text, size_t len, struct wifi_scan *out);

#endif
