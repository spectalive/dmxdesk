// SOURCES: desk_action_log.c
// The exact log lines a sent or dropped toggle/stop-all frame produces,
// captured off stderr the way an operator's terminal or journal would see them.
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "desk_action_log.h"

static void read_all(const char *path, char *out, size_t cap) {
    FILE *f = fopen(path, "r");
    assert(f);
    size_t n = fread(out, 1, cap - 1, f);
    out[n] = '\0';
    fclose(f);
}

int main(void) {
    char path[256];
    snprintf(path, sizeof path, "./output/dmxdesk-action-log-%d", (int)getpid());
    unlink(path);
    FILE *captured = freopen(path, "w", stderr);
    assert(captured);

    desk_action_log_sent("toggle", "10|255", 1234);
    desk_action_log_sent("stop", "STOP_ALL_FUNCTIONS", 1235);
    desk_action_log_dropped("toggle", "10|255", "link down");
    desk_action_log_dropped("toggle", "20|255", "queue full");
    fflush(stderr);

    char text[1024];
    read_all(path, text, sizeof text);
    unlink(path);
    assert(strcmp(text,
        "toggle: sent 10|255 at 1234\n"
        "stop: sent STOP_ALL_FUNCTIONS at 1235\n"
        "toggle: dropped 10|255: link down\n"
        "toggle: dropped 20|255: queue full\n") == 0);

    return 0;
}
