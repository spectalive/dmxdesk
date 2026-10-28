// desk_console.set_host for OSC: a new console address; the socket closes and
// reopens on the next step.
#ifndef OSC_CONSOLE_SET_HOST_H
#define OSC_CONSOLE_SET_HOST_H


void osc_console_set_host(void *ctx, const char *host, int port);

#endif
