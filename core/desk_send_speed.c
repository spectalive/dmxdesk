#include "desk_send_speed.h"

#include "qlc_codec.h"

#include <stdio.h>

void desk_send_speed(struct qlc_session *session, struct speed_action action, int64_t now) {
    char frame[64];
    int n = -1;
    switch (action.kind) {
    case SPEED_ACT_TIME:
    case SPEED_ACT_TIME_BOTH:
        n = qlc_encode_speed_ms(frame, sizeof frame, action.widget_id, action.ms);
        break;
    case SPEED_ACT_FACTOR:
        n = qlc_encode_speed_factor(frame, sizeof frame, action.widget_id, action.factor);
        break;
    case SPEED_ACT_NONE:
        return;
    }
    if (n > 0 && qlc_session_send(session, frame) == 0)
        fprintf(stderr, "speed: sent %s at %lld\n", frame, (long long)now);
    if (action.kind == SPEED_ACT_TIME_BOTH && action.widget_id2 >= 0) {
        n = qlc_encode_speed_ms(frame, sizeof frame, action.widget_id2, action.ms2);
        if (n > 0 && qlc_session_send(session, frame) == 0)
            fprintf(stderr, "speed: sent %s at %lld\n", frame, (long long)now);
    }
}
