#include "osc_console_send.h"

#include "osc_command_messages.h"
#include "osc_link.h"
#include "osc_link_echo.h"
#include "osc_link_send_msg.h"

int osc_console_send(void *ctx, const struct desk_command *cmd) {
    struct osc_link *l = ctx;
    struct osc_msg msgs[2];
    int n = osc_command_messages(&l->map, cmd, msgs);
    if (n < 0)
        return -1;
    for (int i = 0; i < n; i++)
        if (osc_link_send_msg(l, &msgs[i]) != 0)
            return -1;
    osc_link_echo(l, cmd);
    return 0;
}
