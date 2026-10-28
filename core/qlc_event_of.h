// A decoded QLC+ message as a desk event: function state, the grand
// master and a speed dial's echo; anything else is DESK_EVENT_NONE.
#ifndef QLC_EVENT_OF_H
#define QLC_EVENT_OF_H

#include "desk_event.h"
#include "qlc_codec.h"

struct desk_event qlc_event_of(const struct qlc_msg *msg);

#endif
