#include "desk_toggle_drain.h"

#include "desk_send_tagged.h"

void desk_toggle_drain(struct qlc_session *session, struct desk_toggle_queue *toggles, int64_t now) {
    if (!desk_toggle_queue_slot_open(toggles, now))
        return;
    char frame[64];
    if (!desk_toggle_queue_pop(toggles, frame, sizeof frame))
        return;
    desk_toggle_queue_mark_slot(toggles, now);
    desk_send_tagged(session, "toggle", frame, now);
}
