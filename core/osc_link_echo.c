#include "osc_link_echo.h"

#include "osc_command_reported.h"
#include "osc_echo_of.h"
#include "osc_loop_push.h"
#include "osc_map_apply.h"

// A control the console reports gets its reply from the console instead.
// The reply lands in the map at once, so a second toggle before the next
// read flips back rather than on again; reading it later applies the same
// state twice, which changes nothing.
void osc_link_echo(struct osc_link *l, const struct desk_command *cmd) {
    struct osc_msg reply;
    if (osc_command_reported(&l->map, cmd) || !osc_echo_of(&l->map, cmd, &reply))
        return;
    osc_map_apply(&l->map, &reply);
    osc_loop_push(l, &reply);
}
