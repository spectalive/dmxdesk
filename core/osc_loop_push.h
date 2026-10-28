// Queues a looped-back reply; the oldest is dropped when the queue is full,
// since only the latest state of anything matters.
#ifndef OSC_LOOP_PUSH_H
#define OSC_LOOP_PUSH_H

#include "osc_link.h"

void osc_loop_push(struct osc_link *l, const struct osc_msg *msg);

#endif
