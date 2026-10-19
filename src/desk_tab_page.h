// The page a tab opens: DIRECTO and RITMO their one page, EFECTOS the family
// last on screen (the first family until one has been). -1 when the layout
// has no page for that tab, which is then neither painted nor pressed.
#ifndef DESK_TAB_PAGE_H
#define DESK_TAB_PAGE_H

#include "desk_model.h"

int desk_tab_page(const struct desk_model *m, int tab);

#endif
