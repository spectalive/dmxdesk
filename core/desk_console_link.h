// The link to the console as a sequence: READY means connected and holding
// a fresh snapshot of the console taken on this very connection.
#ifndef DESK_CONSOLE_LINK_H
#define DESK_CONSOLE_LINK_H

enum desk_console_link { DESK_CONSOLE_DOWN, DESK_CONSOLE_CONNECTING, DESK_CONSOLE_FETCHING,
                         DESK_CONSOLE_READY };

#endif
