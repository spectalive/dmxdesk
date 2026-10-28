// A message to address carrying one number; 1, or -1 when the address is
// empty, which is a control the console gave no address.
#ifndef OSC_MSG_MAKE_H
#define OSC_MSG_MAKE_H

#include "osc_msg.h"

int osc_msg_make(struct osc_msg *out, const char *address, char type, int i, float f);

#endif
