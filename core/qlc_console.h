// QLC+ as the desk's console: the web-access session behind the desk's
// console table. The session stays the caller's to create and free.
#ifndef QLC_CONSOLE_H
#define QLC_CONSOLE_H

#include "desk_console.h"
#include "qlc_session.h"

struct desk_console qlc_console(struct qlc_session *session);

#endif
