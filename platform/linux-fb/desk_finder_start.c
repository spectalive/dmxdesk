#include "desk_finder_start.h"

#include <stdio.h>
#include <unistd.h>

#include "iface_prefix.h"
#include "linux_desk_paths.h"

#define FIND_TIMEOUT_S 8

int desk_finder_start(struct action_worker *w, const char *exe, const char *addr, int port) {
    int prefix = iface_prefix("wlan0");
    if (prefix < 8 || prefix > 30)
        return -1;
    char cidr[64], port_text[16];
    snprintf(cidr, sizeof cidr, "%.40s/%d", addr, prefix);
    snprintf(port_text, sizeof port_text, "%d", port);
    const char *argv[] = {
        "/bin/sh", "-c", "\"$0\" --find \"$1\" \"$2\" > " FIND_TMP " && mv " FIND_TMP " " FIND_OUT,
        exe, cidr, port_text, NULL,
    };
    unlink(FIND_OUT);
    return aw_start(w, argv, FIND_TIMEOUT_S);
}
