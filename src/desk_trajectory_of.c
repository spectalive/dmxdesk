#include "desk_trajectory_of.h"

#include <string.h>
#include <strings.h>

#include "icon_data.h"

int desk_trajectory_of(const char *caption) {
    // Each shape's three marks follow one another: alone, Simultaneo, Alternado.
    static const struct { const char *word; enum icon_large_id mark; } shapes[] = {
        { "Circulo", ICON_TRAJ_CIRCLE }, { "C\xc3\xadrculo", ICON_TRAJ_CIRCLE },
        { "Ocho", ICON_TRAJ_EIGHT },
        { "Linea", ICON_TRAJ_LINE }, { "L\xc3\xadnea", ICON_TRAJ_LINE },
        { "Diamante", ICON_TRAJ_DIAMOND }, { "Cuadrado", ICON_TRAJ_SQUARE },
        { "Hoja", ICON_TRAJ_LEAF }, { "Lissajous", ICON_TRAJ_LISSAJOUS },
    };
    if (!caption)
        return -1;
    size_t first = strcspn(caption, " ");
    for (size_t i = 0; i < sizeof shapes / sizeof shapes[0]; i++) {
        if (strlen(shapes[i].word) != first || strncasecmp(caption, shapes[i].word, first) != 0)
            continue;
        const char *mode = caption + first + strspn(caption + first, " ");
        size_t len = strcspn(mode, " ");
        if ((len == 10 && strncasecmp(mode, "Simultaneo", len) == 0) ||
            (len == 11 && strncasecmp(mode, "Simult\xc3\xa1neo", len) == 0))
            return shapes[i].mark + 1;
        if (len == 9 && strncasecmp(mode, "Alternado", len) == 0)
            return shapes[i].mark + 2;
        return shapes[i].mark;
    }
    return -1;
}
