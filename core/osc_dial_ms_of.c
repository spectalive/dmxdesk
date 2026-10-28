#include "osc_dial_ms_of.h"

#include "osc_clamp.h"
#include "osc_msg_number.h"

int osc_dial_ms_of(const int range[2], const struct osc_msg *msg) {
    float number = osc_msg_number(msg);
    if (range[0] < 0 || msg->type != 'f')
        return osc_clamp(number, 0, 0x7fffffff);
    float fraction = number < 0.0f ? 0.0f : number > 1.0f ? 1.0f : number;
    return osc_clamp((float)range[0] + fraction * (float)(range[1] - range[0]), range[0], range[1]);
}
