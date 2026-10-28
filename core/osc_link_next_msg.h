// The next message from the console: a looped-back reply first, then a
// datagram off the socket. A datagram is heard only from a console that
// reports its state and only from its own host; anything else, or a
// datagram that is not OSC, reads as a message with an empty address, which
// means nothing. 1 and the message, or 0 when nothing waits.
#ifndef OSC_LINK_NEXT_MSG_H
#define OSC_LINK_NEXT_MSG_H

#include "osc_link.h"

int osc_link_next_msg(struct osc_link *l, struct osc_msg *out);

#endif
