// An OSC address field of a JSON object into dst, left empty when the field
// is absent. -1 when it is present but not a string starting with '/' that
// fits in OSC_ADDRESS_MAX.
#ifndef OSC_JSON_ADDRESS_H
#define OSC_JSON_ADDRESS_H

#include <cjson/cJSON.h>

int osc_json_address(const cJSON *node, const char *name, char *dst);

#endif
