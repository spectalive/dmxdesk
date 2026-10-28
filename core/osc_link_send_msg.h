// One message to the console. 0 sent, -1 not.
#ifndef OSC_LINK_SEND_MSG_H
#define OSC_LINK_SEND_MSG_H

#include "osc_link.h"

int osc_link_send_msg(struct osc_link *l, const struct osc_msg *msg);

#endif
