#include "osc_console_set_host.h"

#include <stdio.h>

#include "osc_link.h"
#include "osc_link_close.h"

void osc_console_set_host(void *ctx, const char *host, int port) {
    struct osc_link *l = ctx;
    snprintf(l->host, sizeof l->host, "%s", host);
    l->port = port;
    osc_link_close(l);
    l->retry_at = 0;
    snprintf(l->reason, sizeof l->reason, "console address changed");
}
