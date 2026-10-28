#include "qlc_osc_channels_write.h"

#include "qlc_osc_channel.h"
#include "qlc_osc_factor_input.h"

int qlc_osc_channels_write(const struct osc_map *map, FILE *out) {
    int lines = 0;
    for (int i = 0; i < map->surface.count; i++) {
        if (!map->address[i][0])
            continue;
        if (fprintf(out, "%d\t0\t%u\t%s\n", map->surface.item[i].id,
                    (unsigned)qlc_osc_channel(map->address[i]), map->address[i]) < 0)
            return -1;
        lines++;
    }
    for (int i = 0; i < map->factor_presses; i++) {
        const struct osc_factor_press *fp = &map->factor_press[i];
        if (fprintf(out, "%d\t%d\t%u\t%s\n", map->surface.item[fp->item].id,
                    qlc_osc_factor_input(fp->factor), (unsigned)qlc_osc_channel(fp->address),
                    fp->address) < 0)
            return -1;
        lines++;
    }
    return lines;
}
