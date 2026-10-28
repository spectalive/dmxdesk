#include "osc_msg_number.h"

float osc_msg_number(const struct osc_msg *msg) {
    if (msg->type == 'i')
        return (float)msg->i;
    if (msg->type == 'f')
        return msg->f;
    return 0.0f;
}
