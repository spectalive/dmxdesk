#include "osc_msg_format.h"

#include <stdio.h>

int osc_msg_format(const struct osc_msg *msg, char *buf, size_t cap) {
    int n = msg->type == 'i' ? snprintf(buf, cap, "%s %d", msg->address, msg->i)
          : msg->type == 'f' ? snprintf(buf, cap, "%s %g", msg->address, (double)msg->f)
          : snprintf(buf, cap, "%s", msg->address);
    return n < 0 || (size_t)n >= cap ? -1 : n;
}
