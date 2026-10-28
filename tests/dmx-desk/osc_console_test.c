// SOURCES: osc_map_load.c osc_map_parse.c osc_map_parse_items.c osc_map_parse_functions.c osc_item_parse.c osc_function_parse.c osc_kind_of.c osc_json_int.c osc_json_bool.c osc_json_address.c osc_map_free.c desk_surface_free.c desk_surface_find.c desk_surface_is_button.c osc_map_target.c osc_map_apply.c osc_clamp.c osc_msg_number.c osc_event_of.c osc_command_messages.c osc_echo_of.c osc_msg_make.c osc_console.c osc_console_pollfds.c osc_console_step.c osc_console_take_snapshot.c osc_console_recv.c osc_console_send.c osc_console_describe.c osc_console_refresh.c osc_console_set_host.c osc_console_last_rtt.c osc_console_reason.c osc_link_new.c osc_link_free.c osc_link_close.c osc_link_open.c osc_resolve.c osc_any_address.c osc_loop_push.c osc_loop_pop.c osc_link_next_msg.c osc_link_send_msg.c osc_link_echo.c osc_encode.c osc_decode.c osc_padded.c osc_be32_read.c osc_be32_write.c osc_msg_format.c desk_surface_copy.c osc_same_host.c
// The OSC console behind the desk's table (2026-10-28), over real UDP on
// loopback: a socket in the test plays the console. The link comes up and
// offers its surface once, a command reaches the console as its messages,
// a console without feedback still shows the desk what it asked for, a
// console with feedback is heard and nobody else is, and a new address
// reopens the link.
#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "desk_surface_find.h"
#include "desk_surface_free.h"
#include "osc_console.h"
#include "osc_decode.h"
#include "osc_encode.h"
#include "osc_link_free.h"
#include "osc_link_new.h"
#include "osc_same_host.h"

static int console_fd;
static int console_port;

static void open_console(void) {
    console_fd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(console_fd >= 0);
    struct sockaddr_in a = { .sin_family = AF_INET, .sin_addr.s_addr = htonl(INADDR_LOOPBACK) };
    assert(bind(console_fd, (struct sockaddr *)&a, sizeof a) == 0);
    socklen_t len = sizeof a;
    assert(getsockname(console_fd, (struct sockaddr *)&a, &len) == 0);
    console_port = ntohs(a.sin_port);
}

// The next datagram at the console, within a second, and where it came from.
static struct osc_msg console_hears(struct sockaddr_in *from) {
    struct pollfd p = { .fd = console_fd, .events = POLLIN };
    assert(poll(&p, 1, 1000) == 1);
    char buf[256];
    socklen_t len = sizeof *from;
    ssize_t n = recvfrom(console_fd, buf, sizeof buf, 0, (struct sockaddr *)from, &len);
    assert(n > 0);
    struct osc_msg m;
    assert(osc_decode(buf, (size_t)n, &m) == 0);
    return m;
}

static int console_quiet(void) {
    struct pollfd p = { .fd = console_fd, .events = POLLIN };
    return poll(&p, 1, 50) == 0;
}

static void console_says(const struct sockaddr_in *to, const char *address, char type, int i) {
    struct osc_msg m;
    memset(&m, 0, sizeof m);
    strcpy(m.address, address);
    m.type = type;
    m.i = i;
    char buf[128];
    int n = osc_encode(&m, buf, sizeof buf);
    assert(n > 0);
    assert(sendto(console_fd, buf, (size_t)n, 0, (const struct sockaddr *)to, sizeof *to) == n);
}

// The desk's own wait: poll its fds, then read one event.
static int desk_reads(struct desk_console *c, struct desk_event *e) {
    struct pollfd fds[2];
    int n = c->pollfds(c->ctx, fds, 2);
    assert(n == 1);
    poll(fds, 1, 1000);
    return c->recv(c->ctx, e);
}

static char *surface_with_feedback(void) {
    static char path[256];
    snprintf(path, sizeof path, "%s/osc-feedback.json", getenv("TMPDIR") ? getenv("TMPDIR") : "/tmp");
    FILE *in = fopen("tests/dmx-desk/fixtures/osc-surface.json", "rb");
    FILE *out = fopen(path, "wb");
    assert(in && out);
    char line[512];
    while (fgets(line, sizeof line, in))
        fputs(strstr(line, "\"feedback\"") ? " \"feedback\": true,\n" : line, out);
    fclose(in);
    fclose(out);
    return path;
}

int main(void) {
    open_console();
    struct osc_link_config cfg = { .surface_path = "tests/dmx-desk/fixtures/osc-surface.json",
                                   .port = console_port };
    strcpy(cfg.host, "127.0.0.1");
    struct osc_link_config missing = cfg;
    missing.surface_path = "/nonexistent/osc.json";
    assert(osc_link_new(&missing) == NULL);

    struct osc_link *link = osc_link_new(&cfg);
    assert(link);
    struct desk_console c = osc_console(link);
    struct desk_surface s;
    struct pollfd fds[2];
    struct desk_event e;

    // Nothing before the first step: no socket, no snapshot, no events.
    assert(c.take_snapshot(c.ctx, &s) == 0);
    assert(c.pollfds(c.ctx, fds, 2) == 0);
    assert(c.recv(c.ctx, &e) == 0);
    assert(c.last_rtt(c.ctx) == -1);

    // The first step opens the socket and offers the surface, once.
    assert(c.step(c.ctx, 1000) == DESK_CONSOLE_READY);
    assert(c.take_snapshot(c.ctx, &s) == 1);
    assert(s.count == 9 && desk_surface_find(&s, 12)->dial_ms == 500);
    desk_surface_free(&s);
    assert(c.take_snapshot(c.ctx, &s) == 0);
    assert(c.step(c.ctx, 1100) == DESK_CONSOLE_READY && c.take_snapshot(c.ctx, &s) == 0);

    // A toggle reaches the console as a press: 1, then 0.
    struct desk_command toggle = { DESK_CMD_TOGGLE, 2, 0 };
    assert(c.send(c.ctx, &toggle) == 0);
    struct sockaddr_in desk_addr;
    struct osc_msg m = console_hears(&desk_addr);
    assert(!strcmp(m.address, "/x/2") && m.type == 'f' && m.f == 1.0f);
    m = console_hears(&desk_addr);
    assert(!strcmp(m.address, "/x/2") && m.f == 0.0f);
    assert(console_quiet());
    char text[128];
    assert(c.describe(c.ctx, &toggle, text, sizeof text) > 0 && !strcmp(text, "/x/2 1; /x/2 0"));
    assert(c.describe(c.ctx, &toggle, text, 8) == -1);

    // This console sends no feedback, so the desk hears what it asked for,
    // and a second toggle before reading turns it back off.
    assert(c.recv(c.ctx, &e) == 1 && e.kind == DESK_EVENT_FUNCTION && e.id == 5 && e.running == 1);
    assert(c.recv(c.ctx, &e) == 0);
    assert(c.send(c.ctx, &toggle) == 0 && c.send(c.ctx, &toggle) == 0);
    assert(c.recv(c.ctx, &e) == 1 && e.running == 0);
    assert(c.recv(c.ctx, &e) == 1 && e.running == 1);
    assert(c.recv(c.ctx, &e) == 0);
    for (int i = 0; i < 4; i++)
        console_hears(&desk_addr);
    struct desk_command speed = { DESK_CMD_SPEED_MS, 12, 900 };
    assert(c.send(c.ctx, &speed) == 0);
    m = console_hears(&desk_addr);
    assert(!strcmp(m.address, "/x/12/ms") && m.type == 'i' && m.i == 900);
    assert(c.recv(c.ctx, &e) == 1 && e.kind == DESK_EVENT_SPEED && e.id == 12 && e.value == 900);

    // A command the surface cannot address is dropped, and nothing goes out.
    struct desk_command stray = { DESK_CMD_TOGGLE, 99, 0 };
    assert(c.send(c.ctx, &stray) == -1 && c.describe(c.ctx, &stray, text, sizeof text) == -1);
    assert(console_quiet());

    // A refresh offers the surface again with the state heard since.
    c.refresh(c.ctx, 1200);
    assert(c.take_snapshot(c.ctx, &s) == 1);
    assert(desk_surface_find(&s, 2)->on == 1 && desk_surface_find(&s, 12)->dial_ms == 900);
    desk_surface_free(&s);

    // A datagram that is not OSC is read and means nothing.
    assert(sendto(console_fd, "junk", 4, 0, (struct sockaddr *)&desk_addr, sizeof desk_addr) == 4);
    assert(desk_reads(&c, &e) == 1 && e.kind == DESK_EVENT_NONE);
    // Nor is a well-formed reply heard from a console declared without
    // feedback: the desk keeps what it asked for.
    console_says(&desk_addr, "/x/12/factor", 'i', 7);
    assert(desk_reads(&c, &e) == 1 && e.kind == DESK_EVENT_NONE);
    c.refresh(c.ctx, 1250);
    assert(c.take_snapshot(c.ctx, &s) == 1 && desk_surface_find(&s, 12)->dial_factor == 3);
    desk_surface_free(&s);

    // A new address closes the socket; the next step reopens it and offers
    // the surface again. No address at all keeps the link down with a reason.
    c.set_host(c.ctx, "127.0.0.1", console_port);
    assert(c.pollfds(c.ctx, fds, 2) == 0 && !strcmp(c.reason(c.ctx), "console address changed"));
    assert(c.step(c.ctx, 1300) == DESK_CONSOLE_READY && c.take_snapshot(c.ctx, &s) == 1);
    desk_surface_free(&s);
    c.set_host(c.ctx, "", console_port);
    assert(c.step(c.ctx, 1400) == DESK_CONSOLE_DOWN);
    assert(!strcmp(c.reason(c.ctx), "no console address"));
    assert(c.step(c.ctx, 1500) == DESK_CONSOLE_DOWN);   // retried later, not every turn
    struct desk_command flash = { DESK_CMD_FLASH, 3, 1 };
    assert(c.send(c.ctx, &flash) == -1);
    osc_link_free(link);

    // A console that reports its state: no echo, and what it says is heard,
    // on the listen port it was told to answer to.
    cfg.surface_path = surface_with_feedback();
    cfg.listen_port = console_port + 1;
    link = osc_link_new(&cfg);
    assert(link);
    c = osc_console(link);
    assert(c.step(c.ctx, 0) == DESK_CONSOLE_READY);
    assert(c.send(c.ctx, &speed) == 0);
    m = console_hears(&desk_addr);
    assert(ntohs(desk_addr.sin_port) == console_port + 1);
    assert(c.recv(c.ctx, &e) == 0);
    console_says(&desk_addr, "/x/12/factor", 'i', 5);
    assert(desk_reads(&c, &e) == 1 && e.kind == DESK_EVENT_SPEED && e.id == 12 && e.value == 500 &&
           e.factor == 5);
    console_says(&desk_addr, "/f/40", 'i', 1);
    assert(desk_reads(&c, &e) == 1 && e.kind == DESK_EVENT_FUNCTION && e.id == 40 && e.running);
    osc_link_free(link);
    osc_link_free(NULL);

    // Only the console's own host is heard, from whatever port.
    struct sockaddr_storage a, b;
    memset(&a, 0, sizeof a);
    memset(&b, 0, sizeof b);
    struct sockaddr_in *a4 = (struct sockaddr_in *)&a, *b4 = (struct sockaddr_in *)&b;
    a4->sin_family = b4->sin_family = AF_INET;
    a4->sin_addr.s_addr = inet_addr("192.168.1.60");
    a4->sin_port = htons(8000);
    b4->sin_addr.s_addr = inet_addr("192.168.1.60");
    b4->sin_port = htons(53001);
    assert(osc_same_host(&a, &b));
    b4->sin_addr.s_addr = inet_addr("192.168.1.61");
    assert(!osc_same_host(&a, &b));
    struct sockaddr_in6 *c6 = (struct sockaddr_in6 *)&b;
    memset(&b, 0, sizeof b);
    c6->sin6_family = AF_INET6;
    assert(!osc_same_host(&a, &b));
    memcpy(&a, &b, sizeof a);
    assert(osc_same_host(&a, &b));
    c6->sin6_addr.s6_addr[15] = 1;
    assert(!osc_same_host(&a, &b));
    unlink(cfg.surface_path);
    close(console_fd);
    return 0;
}
