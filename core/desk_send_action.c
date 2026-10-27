#include "desk_send_action.h"

#include "desk_action_log.h"
#include "desk_release_hook.h"
#include "desk_send_tagged.h"
#include "qlc_codec.h"

void desk_send_action(struct qlc_session *session, struct desk_toggle_queue *toggles,
                      struct desk_model *model, const struct show_map *map,
                      struct desk_action action, int64_t now) {
    char frame[64];
    int n = -1;
    switch (action.kind) {
    case DESK_ACT_TOGGLE: {
        int hook = desk_release_hook(model, map, action.widget_id);
        n = qlc_encode_toggle(frame, sizeof frame, action.widget_id);
        if (n <= 0)
            return;
        if (desk_toggle_queue_push(toggles, frame) != 0) {
            desk_action_log_dropped("toggle", frame, "queue full");
            return;
        }
        desk_note_sent(model, action.widget_id, now);
        if (hook < 0 || qlc_encode_toggle(frame, sizeof frame, hook) <= 0)
            return;
        if (desk_toggle_queue_push(toggles, frame) != 0) {
            desk_action_log_dropped("toggle", frame, "queue full");
            return;
        }
        // Both stay pending until QLC+ answers: a second tap before then is
        // not a second release, and the hook is not pressed off again.
        desk_note_sent(model, hook, now);
        return;
    }
    case DESK_ACT_HAZE_OFF:
        n = qlc_encode_function_status(frame, sizeof frame, action.widget_id, 0);
        if (n <= 0)
            return;
        while (desk_toggle_queue_push(toggles, frame) != 0) {
            char waiting[64];
            if (!desk_toggle_queue_pop(toggles, waiting, sizeof waiting))
                return;
            desk_action_log_dropped("toggle", waiting, "haze off priority");
        }
        return;
    case DESK_ACT_MASTER:
        n = qlc_encode_grand_master(frame, sizeof frame, action.value);
        break;
    case DESK_ACT_STOP_ALL:
        n = qlc_encode_stop_all(frame, sizeof frame, action.widget_id);
        if (n > 0) {
            // A toggle still waiting for its slot would land after the stop
            // and start something again: stop-all empties the queue first.
            char waiting[64];
            while (desk_toggle_queue_pop(toggles, waiting, sizeof waiting))
                desk_action_log_dropped("toggle", waiting, "stop-all");
            desk_send_tagged(session, "stop", frame, now);
        }
        return;
    case DESK_ACT_NONE:
        return;
    }
    if (n > 0)
        qlc_session_send(session, frame);
}
