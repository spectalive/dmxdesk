// The oldest looped-back reply: 1 and the message, or 0 when none waits.
#ifndef OSC_LOOP_POP_H
#define OSC_LOOP_POP_H

#include "osc_link.h"

int osc_loop_pop(struct osc_link *l, struct osc_msg *out);

#endif
