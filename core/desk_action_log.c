#include "desk_action_log.h"

#include <stdio.h>

void desk_action_log_sent(const char *tag, const char *frame, int64_t now) {
    fprintf(stderr, "%s: sent %s at %lld\n", tag, frame, (long long)now);
}

void desk_action_log_dropped(const char *tag, const char *frame, const char *reason) {
    fprintf(stderr, "%s: dropped %s: %s\n", tag, frame, reason);
}
