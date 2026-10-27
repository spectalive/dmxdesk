#include "desk_link_word.h"

const char *desk_link_word(enum qlc_link link) {
    switch (link) {
    case QLC_READY:      return "Linked";
    case QLC_FETCHING:   return "Reading the show";
    case QLC_CONNECTING: return "Connecting";
    default:             return "Not linked";
    }
}
