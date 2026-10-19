#include "desk_rig_name.h"

#include <string.h>

const char *desk_rig_name(const char *label) {
    return strncmp(label, "Rig ", 4) == 0 ? label + 4 : label;
}
