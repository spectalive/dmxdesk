#include "desk_sync_link_word.h"

#include <stdio.h>
#include <string.h>

#include "desk_link_word.h"

void desk_sync_link_word(struct desk_app *app) {
    if (!app->setup.open || strcmp(app->setup.link_word, desk_link_word(app->link)) == 0)
        return;
    snprintf(app->setup.link_word, sizeof app->setup.link_word, "%s", desk_link_word(app->link));
    app->setup.dirty = 1;
}
