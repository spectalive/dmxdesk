#include "showmap_check_priority.h"

#include "desk_surface_find.h"

const char *showmap_check_priority(const struct map_control *control, const struct desk_surface *doc) {
    if (control->priority_widget_id < 0)
        return NULL;
    if (control->role != MAP_ROLE_BURST || control->priority_function_id < 0)
        return "invalid priority cue";
    const struct desk_surface_item *widget = desk_surface_find(doc, control->priority_widget_id);
    if (!widget || widget->kind != SURFACE_FLASH ||
        widget->function_id != control->priority_function_id || widget->disabled)
        return "priority cue mismatch";
    if (widget->flash_max_ms <= 0 || widget->flash_max_ms > control->burst_ms)
        return "master timeout missing";
    if (!widget->flash_priority)
        return "master priority missing";
    return NULL;
}
