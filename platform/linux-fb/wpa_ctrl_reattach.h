#ifndef WPA_CTRL_REATTACH_H
#define WPA_CTRL_REATTACH_H

struct wpa_ctrl;

// Dials the event socket again and sends ATTACH without waiting for the
// reply, for a daemon that restarted and so never heard the first ATTACH.
// The reply arrives on the event socket, where wpa_ctrl_event drops it.
// 0 when ATTACH was sent, -1 with no event socket; never blocks.
int wpa_ctrl_reattach(struct wpa_ctrl *c);

#endif
