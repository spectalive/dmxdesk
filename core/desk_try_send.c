#include "desk_try_send.h"

int desk_try_send(const struct desk_console *console, const struct desk_command *cmd,
                  char *text, size_t cap) {
    text[0] = '\0';
    if (console->describe(console->ctx, cmd, text, cap) <= 0)
        return -1;
    return console->send(console->ctx, cmd) == 0 ? 0 : -1;
}
