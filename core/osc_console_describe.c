#include "osc_console_describe.h"

#include "osc_command_messages.h"
#include "osc_link.h"
#include "osc_msg_format.h"

int osc_console_describe(void *ctx, const struct desk_command *cmd, char *buf, size_t cap) {
    struct osc_link *l = ctx;
    struct osc_msg msgs[2];
    int n = osc_command_messages(&l->map, cmd, msgs);
    if (n < 0 || cap < 1)
        return -1;
    size_t at = 0;
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            if (cap - at < 3)
                return -1;
            buf[at++] = ';';
            buf[at++] = ' ';
        }
        int len = osc_msg_format(&msgs[i], buf + at, cap - at);
        if (len < 0)
            return -1;
        at += (size_t)len;
    }
    return (int)at;
}
