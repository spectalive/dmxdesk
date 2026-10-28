// A [min, max] field of a JSON object into out, left at -1, -1 when the
// field is absent. -1 when it is present but not two integers with
// 0 <= min < max.
#ifndef OSC_JSON_RANGE_H
#define OSC_JSON_RANGE_H

#include <cjson/cJSON.h>

int osc_json_range(const cJSON *node, const char *name, int out[2]);

#endif
