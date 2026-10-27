#include "linux_desk_power_key.h"

#include "linux_desk.h"

int linux_desk_power_key(void *ctx) {
    struct linux_desk *d = ctx;
    return d->power_ready && power_key_read(&d->power_key, d->power_fd);
}
