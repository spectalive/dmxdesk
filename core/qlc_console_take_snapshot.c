#include "qlc_console_take_snapshot.h"

#include "qlc_session.h"
#include "qlc_surface_of.h"

int qlc_console_take_snapshot(void *ctx, struct desk_surface *out) {
    struct vc_doc doc;
    if (!qlc_session_take_snapshot(ctx, &doc))
        return 0;
    int ok = qlc_surface_of(&doc, out) == 0;
    vc_free(&doc);
    return ok;
}
