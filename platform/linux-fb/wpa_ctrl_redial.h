#ifndef WPA_CTRL_REDIAL_H
#define WPA_CTRL_REDIAL_H

struct wpa_ctrl;

// Dials the request socket again when an abandon could not: one attempt,
// at the address abandon chose, never a loop or a wait. 0 with a socket,
// -1 without one.
int wpa_ctrl_redial(struct wpa_ctrl *c);

#endif
