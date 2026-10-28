// SOURCES: qlc_codec.c qlc_encode_command.c qlc_event_of.c qlc_console_link_of.c
// The QLC+ adapter's translations: every desk command is the frame the
// codec writes for it, the three messages the desk acts on become events
// and everything else is ignored, and the link maps state for state.
#include <assert.h>
#include <string.h>

#include "qlc_console_link_of.h"
#include "qlc_encode_command.h"
#include "qlc_event_of.h"

static void same_frame(struct desk_command cmd, const char *expect) {
    char frame[64];
    assert(qlc_encode_command(frame, sizeof frame, &cmd) > 0);
    assert(strcmp(frame, expect) == 0);
}

static struct qlc_msg decoded(const char *frame) {
    struct qlc_msg msg;
    assert(qlc_decode(frame, strlen(frame), &msg) == 0);
    return msg;
}

int main(void) {
    char expect[64];
    qlc_encode_toggle(expect, sizeof expect, 12);
    same_frame((struct desk_command){ DESK_CMD_TOGGLE, 12, 0 }, expect);
    qlc_encode_stop_all(expect, sizeof expect, 20);
    same_frame((struct desk_command){ DESK_CMD_STOP_ALL, 20, 0 }, expect);
    qlc_encode_flash(expect, sizeof expect, 31, 1);
    same_frame((struct desk_command){ DESK_CMD_FLASH, 31, 1 }, expect);
    qlc_encode_flash(expect, sizeof expect, 31, 0);
    same_frame((struct desk_command){ DESK_CMD_FLASH, 31, 0 }, expect);
    qlc_encode_function_status(expect, sizeof expect, 44, 0);
    same_frame((struct desk_command){ DESK_CMD_FUNCTION, 44, 0 }, expect);
    qlc_encode_grand_master(expect, sizeof expect, 200);
    same_frame((struct desk_command){ DESK_CMD_GRAND_MASTER, 0, 200 }, expect);
    qlc_encode_speed_ms(expect, sizeof expect, 33, 504);
    same_frame((struct desk_command){ DESK_CMD_SPEED_MS, 33, 504 }, expect);
    qlc_encode_speed_factor(expect, sizeof expect, 33, 4);
    same_frame((struct desk_command){ DESK_CMD_SPEED_FACTOR, 33, 4 }, expect);
    // A command the grammar refuses stays refused.
    char frame[64];
    struct desk_command bad = { DESK_CMD_TOGGLE, -1, 0 };
    assert(qlc_encode_command(frame, sizeof frame, &bad) < 0);

    struct qlc_msg msg = decoded("FUNCTION|17|Running");
    struct desk_event e = qlc_event_of(&msg);
    assert(e.kind == DESK_EVENT_FUNCTION && e.id == 17 && e.running == 1);
    msg = decoded("GM_VALUE|128|50%");
    e = qlc_event_of(&msg);
    assert(e.kind == DESK_EVENT_GRAND_MASTER && e.value == 128);
    msg = decoded("33|SPEED_STATE|504|4");
    e = qlc_event_of(&msg);
    assert(e.kind == DESK_EVENT_SPEED && e.id == 33 && e.value == 504 && e.factor == 4);
    msg = decoded("12|BUTTON|255");
    e = qlc_event_of(&msg);
    assert(e.kind == DESK_EVENT_NONE);

    assert(qlc_console_link_of(QLC_DOWN) == DESK_CONSOLE_DOWN);
    assert(qlc_console_link_of(QLC_CONNECTING) == DESK_CONSOLE_CONNECTING);
    assert(qlc_console_link_of(QLC_FETCHING) == DESK_CONSOLE_FETCHING);
    assert(qlc_console_link_of(QLC_READY) == DESK_CONSOLE_READY);
    return 0;
}
