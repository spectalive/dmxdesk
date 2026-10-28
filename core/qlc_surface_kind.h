// What a QLC+ virtual console widget is to the desk: a button by its action,
// a slider as a fader, a speed dial as a dial, a frame as a group, anything
// else as other.
#ifndef QLC_SURFACE_KIND_H
#define QLC_SURFACE_KIND_H

#include "desk_surface.h"
#include "vcjson.h"

enum desk_surface_kind qlc_surface_kind(const struct vc_widget *w);

#endif
