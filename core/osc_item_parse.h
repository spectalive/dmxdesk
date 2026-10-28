// One control of a surface file into its item and its addresses. NULL, or
// why the entry is wrong. Every kind but "other" and "group" needs an
// address, since the desk could not drive it otherwise; only a dial takes a
// factorAddress for its multiplier. A dial's ms, factor and maxMs are -1
// when the console does not say, as in desk_surface; the desk checks them.
#ifndef OSC_ITEM_PARSE_H
#define OSC_ITEM_PARSE_H

#include <cjson/cJSON.h>

#include "desk_surface.h"

const char *osc_item_parse(const cJSON *node, struct desk_surface_item *item, char *address,
                           char *factor_address);

#endif
