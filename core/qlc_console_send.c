#include "qlc_console_send.h"

#include "qlc_encode_command.h"
#include "qlc_session.h"

int qlc_console_send(void *ctx, const struct desk_command *cmd) {
    char frame[64];
    if (qlc_encode_command(frame, sizeof frame, cmd) <= 0)
        return -1;
    return qlc_session_send(ctx, frame);
}
