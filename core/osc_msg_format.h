// A message as text for the log: "/address number", or the bare address
// when it carries none. Its length, or -1 when cap is too small.
#ifndef OSC_MSG_FORMAT_H
#define OSC_MSG_FORMAT_H

#include <stddef.h>

#include "osc_msg.h"

int osc_msg_format(const struct osc_msg *msg, char *buf, size_t cap);

#endif
