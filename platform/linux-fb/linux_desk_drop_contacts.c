#include "linux_desk_drop_contacts.h"

#include "linux_desk.h"

int linux_desk_drop_contacts(void *ctx) {
    struct linux_desk *d = ctx;
    if (!d->touch)
        return 0;
    struct touch_event dropped[32];
    return touch_input_cancel_all(d->touch, dropped, 32);
}
