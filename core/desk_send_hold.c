#include "desk_send_hold.h"

#include "desk_burst_base.h"
#include "qlc_codec.h"

#include <stdio.h>

void desk_send_hold(struct desk_hold *hold, struct qlc_session *session, struct hold_action ha,
                    int64_t now) {
    if (ha.widget_id < 0)
        return;
    char frame[64];
    int n = ha.widget_id >= BURST_BASE
          ? qlc_encode_function_status(frame, sizeof frame, ha.widget_id - BURST_BASE, ha.on)
          : qlc_encode_flash(frame, sizeof frame, ha.widget_id, ha.on);
    if (n > 0 && qlc_session_send(session, frame) == 0) {
        fprintf(stderr, "hold: sent %s at %lld\n", frame, (long long)now);
        return;
    }
    fprintf(stderr, "hold: could not send %s at %lld\n", frame, (long long)now);
    // A release that did not go out is still owed: the output may be on.
    if (!ha.on)
        desk_hold_unsent(hold, ha.widget_id);
}
