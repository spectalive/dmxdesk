#ifndef WPA_CTRL_PRIVATE_H
#define WPA_CTRL_PRIVATE_H

struct wpa_ctrl {
    int fd, event_fd;
    char local[108], local_event[108];
    char path[108];
    int generation;
    int pending;
    int event_generation;
    int attach_pending;   // an ATTACH sent by reattach, its OK not yet read
    char directory[108];
};

#endif
