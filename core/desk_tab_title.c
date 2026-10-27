#include "desk_tab_title.h"

const char *desk_tab_title(enum desk_tab tab) {
    switch (tab) {
    case DESK_TAB_DIRECTO: return "DIRECTO";
    case DESK_TAB_EFECTOS: return "EFECTOS";
    case DESK_TAB_RITMO:   return "RITMO";
    default:               return "";
    }
}
