#include "osc_console.h"

#include "osc_console_describe.h"
#include "osc_console_last_rtt.h"
#include "osc_console_pollfds.h"
#include "osc_console_reason.h"
#include "osc_console_recv.h"
#include "osc_console_refresh.h"
#include "osc_console_send.h"
#include "osc_console_set_host.h"
#include "osc_console_step.h"
#include "osc_console_take_snapshot.h"

struct desk_console osc_console(struct osc_link *link) {
    struct desk_console c = {
        .ctx = link,
        .pollfds = osc_console_pollfds,
        .step = osc_console_step,
        .take_snapshot = osc_console_take_snapshot,
        .recv = osc_console_recv,
        .send = osc_console_send,
        .describe = osc_console_describe,
        .refresh = osc_console_refresh,
        .set_host = osc_console_set_host,
        .last_rtt = osc_console_last_rtt,
        .reason = osc_console_reason,
    };
    return c;
}
