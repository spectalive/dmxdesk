#include "qlc_console_describe.h"

#include "qlc_encode_command.h"

int qlc_console_describe(void *ctx, const struct desk_command *cmd, char *buf, size_t cap) {
    (void)ctx;
    return qlc_encode_command(buf, cap, cmd);
}
