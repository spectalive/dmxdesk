#include "qlc_console_recv.h"

#include <string.h>

#include "qlc_codec.h"
#include "qlc_event_of.h"
#include "qlc_session.h"

int qlc_console_recv(void *ctx, struct desk_event *out) {
    char frame[4096];
    if (qlc_session_recv(ctx, frame, sizeof frame) != 1)
        return 0;
    struct qlc_msg msg;
    if (qlc_decode(frame, strlen(frame), &msg) != 0)
        msg.kind = QLC_UNKNOWN;
    *out = qlc_event_of(&msg);
    return 1;
}
