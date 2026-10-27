#include "linux_desk_frame.h"

#include "linux_desk.h"
#include "linux_desk_present.h"
#include "linux_desk_publish.h"
#include "linux_desk_report.h"

int linux_desk_frame(void *ctx, struct desk_app *app, int damaged, int dx, int dy, int dw, int dh,
                     int64_t now) {
    struct linux_desk *d = ctx;
    if (d->qt_pipe)
        linux_desk_publish(d, app, damaged, now);
    else if (linux_desk_present(d, app, damaged, dx, dy, dw, dh, now))
        return 1;
    linux_desk_report(d, app, now);
    return 0;
}
