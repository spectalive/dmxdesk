#include "linux_desk_service.h"

#include "linux_desk.h"
#include "linux_desk_finder.h"
#include "linux_desk_supplicant.h"

void linux_desk_service(void *ctx, struct desk_app *app, int64_t now) {
    struct linux_desk *d = ctx;
    if (d->wpa)
        linux_desk_supplicant(d, app, now);
    linux_desk_finder(d, app);
}
