#include "qlc_console.h"

#include "qlc_console_describe.h"
#include "qlc_console_last_rtt.h"
#include "qlc_console_pollfds.h"
#include "qlc_console_reason.h"
#include "qlc_console_recv.h"
#include "qlc_console_refresh.h"
#include "qlc_console_send.h"
#include "qlc_console_set_host.h"
#include "qlc_console_step.h"
#include "qlc_console_take_snapshot.h"

struct desk_console qlc_console(struct qlc_session *session) {
    struct desk_console c = {
        .ctx = session,
        .pollfds = qlc_console_pollfds,
        .step = qlc_console_step,
        .take_snapshot = qlc_console_take_snapshot,
        .recv = qlc_console_recv,
        .send = qlc_console_send,
        .describe = qlc_console_describe,
        .refresh = qlc_console_refresh,
        .set_host = qlc_console_set_host,
        .last_rtt = qlc_console_last_rtt,
        .reason = qlc_console_reason,
    };
    return c;
}
