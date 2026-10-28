#include "desk_link_of.h"

enum desk_link desk_link_of(enum desk_console_link link, int have_master) {
    switch (link) {
    case DESK_CONSOLE_READY:      return DESK_LINK_READY;
    case DESK_CONSOLE_FETCHING:   return DESK_LINK_SYNCING;
    case DESK_CONSOLE_CONNECTING: return DESK_LINK_CONNECTING;
    default:                      return have_master ? DESK_LINK_CONNECTING : DESK_LINK_DOWN;
    }
}
