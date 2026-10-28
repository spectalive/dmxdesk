#include "qlc_console_link_of.h"

enum desk_console_link qlc_console_link_of(enum qlc_link link) {
    switch (link) {
    case QLC_READY:      return DESK_CONSOLE_READY;
    case QLC_FETCHING:   return DESK_CONSOLE_FETCHING;
    case QLC_CONNECTING: return DESK_CONSOLE_CONNECTING;
    default:             return DESK_CONSOLE_DOWN;
    }
}
