// The networks a Wi-Fi scan found, as the desk shows them: one entry per
// SSID with its signal and the security that decides whether it can be
// joined. The platform fills it; the setup screen only reads it.
#ifndef WIFI_NETWORK_H
#define WIFI_NETWORK_H

#define WIFI_SCAN_MAX 32
#define WIFI_SSID_MAX 33    // 32 bytes and a NUL, the standard's limit

enum wifi_security { WIFI_OPEN, WIFI_PSK, WIFI_SAE_ONLY, WIFI_EAP };

struct wifi_network {
    char ssid[WIFI_SSID_MAX];
    int level_dbm;
    enum wifi_security security;
};

struct wifi_scan {
    struct wifi_network network[WIFI_SCAN_MAX];
    int count;
};

// Whether the desk can join it: open or WPA-PSK.
int wifi_scan_joinable(enum wifi_security security);

#endif
