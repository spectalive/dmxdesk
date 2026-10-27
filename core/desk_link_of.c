#include "desk_link_of.h"

enum desk_link desk_link_of(enum qlc_link link, int have_master) {
    switch (link) {
    case QLC_READY:      return DESK_LINK_READY;
    case QLC_FETCHING:   return DESK_LINK_SYNCING;
    case QLC_CONNECTING: return DESK_LINK_CONNECTING;
    default:             return have_master ? DESK_LINK_CONNECTING : DESK_LINK_DOWN;
    }
}
