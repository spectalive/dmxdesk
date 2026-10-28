#include "osc_link_echo.h"

#include "osc_echo_of.h"
#include "osc_loop_push.h"
#include "osc_map_apply.h"

// The reply lands in the map at once, so a second toggle before the next
// read flips back rather than on again; reading it later applies the same
// state twice, which changes nothing.
void osc_link_echo(struct osc_link *l, const struct desk_command *cmd) {
    struct osc_msg reply;
    if (l->map.feedback || !osc_echo_of(&l->map, cmd, &reply))
        return;
    osc_map_apply(&l->map, &reply);
    osc_loop_push(l, &reply);
}
