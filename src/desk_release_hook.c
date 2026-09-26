#include "desk_release_hook.h"

static const struct desk_control *cue_of(const struct desk_model *model, int widget_id) {
    for (int i = 0; i < model->count; i++)
        if (model->control[i].kind == DESK_CUE && model->control[i].widget_id == widget_id)
            return &model->control[i];
    return NULL;
}

int desk_release_hook(const struct desk_model *model, const struct show_map *map, int widget_id) {
    const struct map_control *pick = NULL;
    for (int i = 0; i < map->count; i++)
        if (map->control[i].widget_id == widget_id)
            pick = &map->control[i];
    const struct desk_control *tapped = cue_of(model, widget_id);
    if (!pick || pick->releases == 0 || !tapped || tapped->state != DESK_ON)
        return -1;
    int room = -1;
    for (int i = 0; i < model->count && room < 0; i++) {
        const struct desk_control *c = &model->control[i];
        if (c->kind == DESK_CUE && c->role == MAP_ROLE_STATE && c->state == DESK_ON)
            room = c->widget_id;
    }
    if (room < 0)
        return -1;
    for (int e = 0; e < pick->releases; e++) {
        if (pick->release_to[e].state_widget != room)
            continue;
        const struct desk_control *hook = cue_of(model, pick->release_to[e].hook_widget);
        if (!hook || !hook->enabled || hook->state != DESK_OFF)
            return -1;
        return hook->widget_id;
    }
    return -1;
}
