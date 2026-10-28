#include "desk_link_word.h"

const char *desk_link_word(enum desk_console_link link) {
    switch (link) {
    case DESK_CONSOLE_READY:      return "Linked";
    case DESK_CONSOLE_FETCHING:   return "Reading the show";
    case DESK_CONSOLE_CONNECTING: return "Connecting";
    default:                      return "Not linked";
    }
}
