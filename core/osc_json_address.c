#include "osc_json_address.h"

#include <string.h>

#include "osc_msg.h"

int osc_json_address(const cJSON *node, const char *name, char *dst) {
    dst[0] = '\0';
    const cJSON *field = cJSON_GetObjectItemCaseSensitive(node, name);
    if (!field)
        return 0;
    if (!cJSON_IsString(field) || field->valuestring[0] != '/' ||
        strlen(field->valuestring) >= OSC_ADDRESS_MAX)
        return -1;
    strcpy(dst, field->valuestring);
    return 0;
}
