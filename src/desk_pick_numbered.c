#include "desk_pick_numbered.h"

#include <string.h>

#include "desk_caption_number.h"

int desk_pick_numbered(const struct show_map *map, const struct map_section *sec, int control) {
    const char *own = desk_caption_number(map->control[control].caption);
    if (map->control[control].role != MAP_ROLE_PICK || !own)
        return 0;
    size_t own_stem = (size_t)(own - map->control[control].caption);
    // The stem shared by the most numbered picks; the earliest wins a tie.
    int best = -1, best_count = 0;
    for (int i = sec->first; i < sec->first + sec->count; i++) {
        const char *number = desk_caption_number(map->control[i].caption);
        if (map->control[i].role != MAP_ROLE_PICK || !number)
            continue;
        size_t stem = (size_t)(number - map->control[i].caption);
        int count = 0;
        for (int j = sec->first; j < sec->first + sec->count; j++) {
            const char *other = desk_caption_number(map->control[j].caption);
            if (map->control[j].role == MAP_ROLE_PICK && other &&
                (size_t)(other - map->control[j].caption) == stem &&
                strncmp(map->control[j].caption, map->control[i].caption, stem) == 0)
                count++;
        }
        if (count > best_count) {
            best = i;
            best_count = count;
        }
    }
    if (best_count < 2)
        return 0;
    size_t stem = (size_t)(desk_caption_number(map->control[best].caption) - map->control[best].caption);
    return stem == own_stem && strncmp(map->control[best].caption, map->control[control].caption, stem) == 0;
}
