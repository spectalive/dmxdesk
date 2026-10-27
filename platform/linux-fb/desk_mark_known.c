#include "desk_mark_known.h"

#include "linux_desk_paths.h"
#include "wifi_conf.h"

void desk_mark_known(struct desk_setup *setup, const struct wifi_scan *scan) {
    struct wifi_conf conf;
    int known[WIFI_SCAN_MAX] = { 0 };
    if (wifi_conf_read(WIFI_CONF_PATH, &conf) >= 0)
        for (int i = 0; i < scan->count; i++)
            known[i] = wifi_conf_knows(&conf, scan->network[i].ssid);
    desk_setup_set_scan(setup, scan, known);
}
