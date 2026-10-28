// One QLC+ widget as an item of an OSC surface file for QLC+'s own OSC
// input, or NULL when memory runs out. The item carries every field of its
// desk_surface item; a control the desk drives gets the address
// <prefix>/<id>. A widget hidden when /vc.json was served is disabled,
// since QLC+ ignores external input to a widget on a hidden frame page. A
// speed dial takes its time as 0..1 of its own range when it has one, its
// multiplier as one button per factor at <prefix>/<id>/x<factor>, and, like
// a fader, "feedback": false, since QLC+ 5 reports neither back.
#ifndef QLC_OSC_ITEM_JSON_H
#define QLC_OSC_ITEM_JSON_H

#include <cjson/cJSON.h>

#include "desk_surface.h"
#include "vcjson.h"

cJSON *qlc_osc_item_json(const struct vc_widget *w, const struct desk_surface_item *it,
                         const char *prefix, int feedback);

#endif
