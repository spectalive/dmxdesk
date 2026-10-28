// One function entry of a surface file. NULL, or why it is wrong.
#ifndef OSC_FUNCTION_PARSE_H
#define OSC_FUNCTION_PARSE_H

#include <cjson/cJSON.h>

#include "osc_function.h"

const char *osc_function_parse(const cJSON *node, struct osc_function *out);

#endif
