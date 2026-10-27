// DMXDESK_RTT: every heartbeat's round trip, once per new reading - the raw
// material for telling a dead master from a slow Wi-Fi.
#ifndef DESK_LOG_RTT_H
#define DESK_LOG_RTT_H

#include "desk_app.h"

void desk_log_rtt(struct desk_app *app);

#endif
