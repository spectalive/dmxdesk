// A boolean field of a JSON object into *out as 1 or 0, which keeps its
// value when the field is absent. -1 when it is present but not a boolean.
#ifndef OSC_JSON_BOOL_H
#define OSC_JSON_BOOL_H

#include <cjson/cJSON.h>

int osc_json_bool(const cJSON *node, const char *name, int *out);

#endif
