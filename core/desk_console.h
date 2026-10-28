// The lighting console the desk drives, behind one table of calls. The desk
// steps it, takes its snapshot, reads its events and sends it commands; an
// adapter (qlc_console for QLC+) fills the table and passes its own state as
// ctx. Nothing here blocks: the loop polls the fds it hands out.
//
// The snapshot is a desk_surface: the adapter describes the console's
// controls in the desk's own terms (qlc_surface_of for QLC+'s /vc.json).
#ifndef DESK_CONSOLE_H
#define DESK_CONSOLE_H

#include <poll.h>
#include <stddef.h>
#include <stdint.h>

#include "desk_command.h"
#include "desk_console_link.h"
#include "desk_event.h"
#include "desk_surface.h"

struct desk_console {
    void *ctx;
    // Fills up to cap pollfds and returns how many.
    int (*pollfds)(void *ctx, struct pollfd *fds, int cap);
    // Retries, timeouts, heartbeats: everything time-driven. Returns the link.
    enum desk_console_link (*step)(void *ctx, int64_t now);
    // 1 when a fresh snapshot is waiting; the caller owns out afterwards.
    int (*take_snapshot)(void *ctx, struct desk_surface *out);
    // 1 and an event, or 0 when none is waiting.
    int (*recv)(void *ctx, struct desk_event *out);
    // 0 sent, -1 dropped: never kept for later, because a toggle sent late
    // is a second toggle.
    int (*send)(void *ctx, const struct desk_command *cmd);
    // The command as the console's wire will carry it, for the log. Its
    // length, or -1 for a command the console cannot express.
    int (*describe)(void *ctx, const struct desk_command *cmd, char *buf, size_t cap);
    // Re-reads the console's state on the open link.
    void (*refresh)(void *ctx, int64_t now);
    // Changes the console's address; the link drops and redials.
    void (*set_host)(void *ctx, const char *host, int port);
    // The last heartbeat's round trip in ms, or -1 before the first.
    int (*last_rtt)(void *ctx);
    // Why the link is down, or why it was last lost.
    const char *(*reason)(void *ctx);
};

#endif
