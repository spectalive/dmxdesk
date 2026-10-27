// A frame refused because the link went down between the touch and the send
// is dropped, never kept: a toggle sent late is a second toggle.
#ifndef DESK_SEND_TAGGED_H
#define DESK_SEND_TAGGED_H

#include "qlc_session.h"

#include <stdint.h>

void desk_send_tagged(struct qlc_session *session, const char *tag, const char *frame, int64_t now);

#endif
