#include "showmap_check_priority.h"

const char *showmap_check_priority(const struct map_control *control, const struct vc_doc *doc) {
    if (control->priority_widget_id < 0)
        return NULL;
    if (control->role != MAP_ROLE_BURST || control->priority_function_id < 0)
        return "invalid priority cue";
    const struct vc_widget *widget = vc_find(doc, control->priority_widget_id);
    if (!widget || widget->type_id != VC_BUTTON || widget->action_type != VC_FLASH ||
        widget->function_id != control->priority_function_id || widget->disabled)
        return "priority cue mismatch";
    if (widget->flash_max_hold_ms <= 0 || widget->flash_max_hold_ms > control->burst_ms)
        return "master timeout missing";
    if (!widget->flash_override || !widget->flash_force_ltp)
        return "master priority missing";
    return NULL;
}
