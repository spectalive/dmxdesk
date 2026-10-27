#include "desk_tab_page.h"

#include "desk_tab.h"

int desk_tab_page(const struct desk_model *m, int tab) {
    if (tab == DESK_TAB_EFECTOS) {
        int page = m->effects_page;
        if (page >= 0 && page < m->layout.pages && m->layout.tab[page] == DESK_TAB_EFECTOS)
            return page;
        return m->layout.families > 0 ? m->layout.family_page[0] : -1;
    }
    for (int page = 0; page < m->layout.pages; page++)
        if (m->layout.tab[page] == tab)
            return page;
    return -1;
}
