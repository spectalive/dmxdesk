#include "desk_send_tagged.h"

#include "desk_action_log.h"

void desk_send_tagged(struct qlc_session *session, const char *tag, const char *frame, int64_t now) {
    if (qlc_session_send(session, frame) == 0)
        desk_action_log_sent(tag, frame, now);
    else
        desk_action_log_dropped(tag, frame, "link down");
}
