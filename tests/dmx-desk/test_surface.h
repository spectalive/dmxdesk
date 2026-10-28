// The QLC+ document a test loads or builds, as the surface the desk reads:
// converted through the QLC+ adapter on every call, so a test that edits a
// widget and builds again sees the edit, and every model test also runs the
// adapter.
#ifndef TEST_SURFACE_H
#define TEST_SURFACE_H

#include <stdlib.h>

#include "desk_surface_free.h"
#include "qlc_surface_of.h"

static inline const struct desk_surface *test_surface(const struct vc_doc *doc) {
    static struct desk_surface surface;
    desk_surface_free(&surface);
    if (qlc_surface_of(doc, &surface) != 0)
        abort();
    return &surface;
}

#endif
