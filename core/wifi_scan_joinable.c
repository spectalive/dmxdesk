#include "wifi_network.h"

int wifi_scan_joinable(enum wifi_security security) {
    return security == WIFI_OPEN || security == WIFI_PSK;
}
