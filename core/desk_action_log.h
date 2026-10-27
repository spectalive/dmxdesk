// The desk's own record of what it sent, one line per gesture: a toggle or
// stop-all frame that went out, or one that was dropped because the link was
// down or the spacing queue was full. Master-fader moves are continuous and
// are never logged here.
#ifndef DESK_ACTION_LOG_H
#define DESK_ACTION_LOG_H

#include <stdint.h>

// "<tag>: sent <frame> at <now>", e.g. "toggle: sent 10|255 at 1234".
void desk_action_log_sent(const char *tag, const char *frame, int64_t now);
// "<tag>: dropped <frame>: <reason>", e.g. "toggle: dropped 10|255: link down".
void desk_action_log_dropped(const char *tag, const char *frame, const char *reason);

#endif
