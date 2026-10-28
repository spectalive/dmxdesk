// QLC+'s /vc.json as an OSC surface file for QLC+'s own OSC input
// (format in osc_map_parse.h), the text malloc'd and owned by the caller,
// or NULL when memory runs out. Each widget becomes an item through
// qlc_osc_item_json, under the addresses <prefix>/<id>. grand_master names
// the GrandMaster slider's id, or -1; feedback says the workspace sends
// QLC+'s feedback to the desk.
#ifndef QLC_OSC_SURFACE_JSON_H
#define QLC_OSC_SURFACE_JSON_H

#include "vcjson.h"

char *qlc_osc_surface_json(const struct vc_doc *doc, const char *prefix, int grand_master,
                           int feedback);

#endif
