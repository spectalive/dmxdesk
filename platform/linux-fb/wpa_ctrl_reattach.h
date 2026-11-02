#ifndef WPA_CTRL_REATTACH_H
#define WPA_CTRL_REATTACH_H

struct wpa_ctrl;

// Moves the event socket to a fresh address and sends ATTACH without
// waiting for the reply, for a daemon that may have restarted and so never
// heard the first ATTACH; the old socket sends DETACH first, so a daemon
// that did not restart drops the old address. The reply arrives on the
// event socket, where wpa_ctrl_event drops it.
// 0 when ATTACH was sent, -1 with no event socket; never blocks.
int wpa_ctrl_reattach(struct wpa_ctrl *c);

#endif
