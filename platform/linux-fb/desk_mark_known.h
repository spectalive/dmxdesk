// The known networks, matched against a scan for the card's "known" tag.
#ifndef DESK_MARK_KNOWN_H
#define DESK_MARK_KNOWN_H

#include "desk_setup.h"
#include "wifi_network.h"

void desk_mark_known(struct desk_setup *setup, const struct wifi_scan *scan);

#endif
