// An integer field of a JSON object into *out, which keeps its value when
// the field is absent. -1 when it is present but not an integer in
// [min, max].
#ifndef OSC_JSON_INT_H
#define OSC_JSON_INT_H

#include <cjson/cJSON.h>

int osc_json_int(const cJSON *node, const char *name, int min, int max, int *out);

#endif
