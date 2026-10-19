#include "desk_speed_inert_mask.h"

#include "desk_speed_target_rect.h"

void desk_speed_inert_mask(const struct desk_speed *s, unsigned char *mask) {
    // The dial readouts and card backgrounds are inert; tap and step cells
    // alone act. Disabled targets are intentionally inert too.
    for (int i = 0; i < s->dials && i < 2; i++)
        for (int t = SPEED_T_TAP; t <= SPEED_T_DOUBLE; t++)
            if (desk_speed_target_enabled(s, i, (enum speed_target)t))
                desk_region_mark(mask, desk_speed_target_rect(i, (enum speed_target)t), 0);
    if (s->dials >= 2 && desk_speed_target_enabled(s, 0, SPEED_T_BOTH))
        desk_region_mark(mask, desk_speed_target_rect(-1, SPEED_T_BOTH), 0);
}
