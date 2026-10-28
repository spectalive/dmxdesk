// The next message from the console: a looped-back reply first, then a
// datagram off the socket. 1 and the message, whose address is empty when
// the datagram was not OSC; 0 when nothing waits.
#ifndef OSC_LINK_NEXT_MSG_H
#define OSC_LINK_NEXT_MSG_H

#include "osc_link.h"

int osc_link_next_msg(struct osc_link *l, struct osc_msg *out);

#endif
