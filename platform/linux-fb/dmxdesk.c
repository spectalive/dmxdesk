// The lighting desk: the tablet as a control surface for the QLC+ show
// running on the Mac. It draws first, then dials: the session fetches the
// console the master has loaded, the show map is checked against it, the
// tiles are drawn, and one message goes out per gesture. What lights up is
// what the master says is running.
//
//   dmxdesk [--host 192.168.1.50] --map /etc/taq102/show-map.json
//   dmxdesk --qt-pipe --map /usr/share/dmxdesk/vibra.desk.json
//
// The master comes from --host, else from /data/desk.conf, which the gear in
// the status bar writes: the settings surface joins a Wi-Fi, finds a running
// QLC+ on the subnet or takes a typed address, and sets the brightness.
// `dmxdesk --find 192.168.1.71/24 9999` is the sweep, run as a child.
//
// DMXDESK_DUMP=<file.ppm> writes the first frame and exits, so the screen can
// be checked from a laptop without a camera. SIGUSR1 writes the same file
// without stopping, which is how a running desk is photographed.
// DMXDESK_FLIP=1 repaints and flips on every loop, whether or not anything
// changed: a diagnostic for telling a fault in the flip path from one in the
// picture. The desk prints how many frames it flipped every ten seconds, so
// "it is static" is a number rather than an assumption.
#define _GNU_SOURCE
#include <fcntl.h>
#include <linux/input.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "desk_app.h"
#include "desk_build_holds.h"
#include "desk_heartbeat_ms.h"
#include "desk_layout.h"
#include "desk_platform.h"
#include "desk_run.h"
#include "showmap_validate.h"
#include "linux_desk.h"
#include "linux_desk_display.h"
#include "linux_desk_drop_contacts.h"
#include "linux_desk_frame.h"
#include "linux_desk_now.h"
#include "linux_desk_paths.h"
#include "linux_desk_power_key.h"
#include "linux_desk_queue_scan.h"
#include "linux_desk_service.h"
#include "linux_desk_setup_action.h"
#include "linux_desk_setup_opened.h"
#include "linux_desk_status.h"
#include "linux_desk_stopping.h"
#include "linux_desk_touches.h"
#include "linux_desk_wait.h"
#include "loop_trace.h"
#include "master_find.h"
#include "qlc_console.h"
#include "touch_flip.h"

#define VC_LIMIT (1024 * 1024)
// The master pushes only when something changes, and its own ping is every
// five seconds, so the desk asks a question of its own well inside the window
// it treats as stale.
#define STALE_MS 750
#define RECONNECT_MS 1500
#define CONNECT_TIMEOUT_MS 3000
#define FETCH_TIMEOUT_MS 4000

static volatile sig_atomic_t stop;
static void on_signal(int sig) { (void)sig; stop = 1; }

static volatile sig_atomic_t snapshot;
static void on_snapshot(int sig) { (void)sig; snapshot = 1; }

// Both are large and live for the whole process.
static struct desk_app app;
static struct linux_desk desk;

int main(int argc, char **argv) {
    const char *host = NULL;
    int port = 9999;
    const char *map_path = "/etc/taq102/vibra.desk.json";
    const char *card = "/dev/dri/card0";
    const char *touch_device = "/dev/input/event1";
    const char *power_device = NULL;
    int view_page = 0, view_bank = 0;    // --view P,B: the first page shown, for dumps
    int start_setup = 0;                 // --setup: the surface open at start, for dumps
    int qt_pipe = 0;

    if (argc == 4 && !strcmp(argv[1], "--find")) {
        char *end;
        long port = strtol(argv[3], &end, 10);
        if (!argv[3][0] || *end || port < 1 || port > 65535) {
            fprintf(stderr, "find: invalid configured port\n");
            return 2;
        }
        if (master_find_run(argv[2], (int)port, stdout) < 0) {
            perror("find: could not sweep");
            return 2;
        }
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--host") && i + 1 < argc) host = argv[++i];
        else if (!strcmp(argv[i], "--port") && i + 1 < argc) port = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--map") && i + 1 < argc) map_path = argv[++i];
        else if (!strcmp(argv[i], "--card") && i + 1 < argc) card = argv[++i];
        else if (!strcmp(argv[i], "--touch") && i + 1 < argc) touch_device = argv[++i];
        else if (!strcmp(argv[i], "--power") && i + 1 < argc) power_device = argv[++i];
        else if (!strcmp(argv[i], "--setup")) start_setup = 1;
        else if (!strcmp(argv[i], "--qt-pipe")) qt_pipe = 1;
        else if (!strcmp(argv[i], "--view") && i + 1 < argc) {
            if (sscanf(argv[++i], "%d,%d", &view_page, &view_bank) != 2)
                view_page = view_bank = 0;
        }
        else {
            fprintf(stderr, "usage: %s [--host H] [--port P] [--map FILE]"
                            " [--card /dev/dri/cardN] [--touch /dev/input/eventN]"
                            " [--power /dev/input/eventN] [--view PAGE,BANK] [--setup] [--qt-pipe]\n"
                            "       %s --find ADDR/PREFIX PORT\n",
                    argv[0], argv[0]);
            return 2;
        }
    }
    // --host is for the bench; the tablet keeps its master in its own file.
    app.conf_path = DESK_CONF_PATH;
    desk_conf_defaults(&app.conf);
    if (desk_conf_load(&app.conf, DESK_CONF_PATH) != 0)
        fprintf(stderr, "desk: cannot read %s\n", DESK_CONF_PATH);
    if (host) {
        snprintf(app.conf.master, sizeof app.conf.master, "%s", host);
        app.conf.port = port;
    }
    if (!desk_conf_valid_port(app.conf.port)) {
        fprintf(stderr, "dmxdesk: bad port\n");
        return 2;
    }
    if (!app.conf.master[0])
        fprintf(stderr, "desk: no master set; the gear in the status bar sets one\n");

    if (showmap_load(map_path, &app.map) != 0)
        return 1;

    // The display first. The model is built against an empty console, so
    // every tile is drawn disabled with its reason before the network is
    // touched; the session's first snapshot rebuilds it.
    desk.qt_pipe = qt_pipe;
    desk.present = qt_pipe ? NULL : present_open(card);
    if (!qt_pipe && !desk.present)
        return 1;
    int w = qt_pipe ? DESK_W : present_width(desk.present);
    int h = qt_pipe ? DESK_H : present_height(desk.present);
    app.canvas = (struct canvas){ .px = calloc((size_t)w * h, 4), .w = w, .h = h };
    if (!app.canvas.px) {
        present_close(desk.present);
        return 1;
    }
    if (desk_fonts_open(&app.fonts, "/usr/share/fonts/taq102") != 0)
        fprintf(stderr, "desk: fonts missing under /usr/share/fonts/taq102; 3x5 glyphs it is\n");

    int enabled = showmap_build(&app.model, &app.map, &app.console);
    printf("desk: %s, %d of %d controls enabled before the first snapshot\n",
           app.map.key, enabled, app.map.count);
    // The map's controls come first in the model, then the master and the
    // panic button, which is what the resolver is told.
    if (desk_layout_resolve(&app.map, app.map.count, app.map.count + 1, app.map.count + 2,
                            app.map.count + 3, &app.layout) != 0) {
        fprintf(stderr, "desk: the map does not fit the screen\n");
        return 1;
    }
    desk_set_layout(&app.model, &app.layout);
    desk_set_view(&app.model, view_page, view_bank);
    desk_input_init(&app.input);
    desk_speed_init(&app.speed, &app.map);
    app.speed_slot = app.setup_slot = app.gear_slot = -1;
    app.last_page = app.model.page;
    desk_build_holds(&app.hold, &app.model, app.hold_owner);

    // The controller reports in its own units on the mainline driver and in
    // screen pixels on the vendor one, so its declared maxima decide the
    // scale rather than a constant. The desk never turns the panel over, so
    // no axis is flipped: the mode is "none" and the log says so, rather than
    // naming axes (DMXDESK_TOUCH_FLIP used to) that nothing ever applied.
    desk.touch_fd = qt_pipe ? -1 : open(touch_device, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    struct touch_flip flip;
    if (desk.touch_fd >= 0) {
        int declared_x = 0, declared_y = 0;
        struct input_absinfo ai;
        if (ioctl(desk.touch_fd, EVIOCGABS(ABS_MT_POSITION_X), &ai) == 0)
            declared_x = ai.maximum;
        if (ioctl(desk.touch_fd, EVIOCGABS(ABS_MT_POSITION_Y), &ai) == 0)
            declared_y = ai.maximum;
        touch_flip_configure(&flip, w, h, declared_x, declared_y, "none");
        desk.touch = touch_input_new(&flip);
        printf("touch: declared %dx%d, mapping %dx%d, scale %.3fx%.3f, flip %s\n",
               declared_x, declared_y, w, h, flip.scale_x, flip.scale_y,
               flip.name);
    }
    if (!desk.touch)
        fprintf(stderr, "desk: %s\n", qt_pipe ? "touch comes from Qt pipe"
                : "physical touch unavailable");
    if (qt_pipe) {
        int flags = fcntl(STDIN_FILENO, F_GETFL);
        if (flags < 0 || fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) != 0)
            return 1;
        setvbuf(stdout, NULL, _IONBF, 0);
    }

    // The power key blanks and locks; without a node the desk simply never
    // blanks, which is show mode anyway.
    desk.power_fd = power_device ? open(power_device, O_RDONLY | O_NONBLOCK | O_CLOEXEC) : -1;
    if (power_device && desk.power_fd < 0)
        fprintf(stderr, "no power key on %s: the desk will not blank\n", power_device);
    desk_lock_init(&app.lock);

    signal(SIGTERM, on_signal);
    signal(SIGINT, on_signal);
    signal(SIGUSR1, on_snapshot);
    desk.stop = &stop;
    desk.snapshot = &snapshot;

    struct qlc_session_config cfg = {
        .port = port, .heartbeat_ms = DESK_HEARTBEAT_MS, .stale_ms = STALE_MS,
        .reconnect_ms = RECONNECT_MS, .connect_timeout_ms = CONNECT_TIMEOUT_MS,
        .fetch_timeout_ms = FETCH_TIMEOUT_MS, .snapshot_limit = VC_LIMIT,
    };
    snprintf(cfg.host, sizeof cfg.host, "%s", app.conf.master);
    cfg.port = app.conf.port;
    struct qlc_session *session = qlc_session_new(&cfg);
    if (!session)
        return 1;
    app.master = qlc_console(session);

    // The settings surface and what it drives: the backlight, the
    // supplicant, the join, the finder. Each is optional; the card says so.
    desk.start_ms = linux_desk_now(&desk);
    if (desk_power_init(&desk.power, SETTINGS_PATH, "/sys/class/backlight", desk.start_ms) != 0)
        return 1;
    desk_setup_init(&app.setup);
    app.setup.brightness_max = desk.power.max;
    app.setup.brightness = desk.power.level;
    app.setup.power_aware = desk_power_aware(&desk.power);
    desk_setup_set_master(&app.setup, app.conf.master, app.conf.port, app.conf.master[0] != '\0');
    int64_t attach_at = loop_trace(NULL, 0);
    desk.wpa = wpa_ctrl_open(WPA_SOCKET);
    loop_trace("wpa_ctrl_open", attach_at);
    app.setup.wifi_available = desk.wpa != NULL;
    if (!desk.wpa)
        fprintf(stderr, "desk: no supplicant control at %s: the Wi-Fi card is read-only\n", WPA_SOCKET);
    wifi_join_init(&desk.join, desk.wpa, WIFI_CONF_PATH);
    desk.finder = aw_new();
    if (!desk.finder)
        return 1;
    ssize_t exe_len = readlink("/proc/self/exe", desk.exe, sizeof desk.exe - 1);
    if (exe_len > 0)
        desk.exe[exe_len] = '\0';
    else
        snprintf(desk.exe, sizeof desk.exe, "%s", argv[0]);
    if (start_setup) {
        desk_setup_open(&app.setup);
        if (desk.wpa)
            linux_desk_queue_scan(&desk, &app, desk.start_ms);
    }

    desk.dump = getenv("DMXDESK_DUMP");
    // DMXDESK_RTT=1 prints every heartbeat's round trip: the raw material for
    // deciding whether 750 ms of silence is a dead master or a slow Wi-Fi.
    app.log_rtt = getenv("DMXDESK_RTT") != NULL;
    app.last_rtt_logged = -1;
    // DMXDESK_PERF=1 keeps the last thousand paint and present times and
    // prints their p50/p95 with RSS and MemAvailable every ten seconds: the
    // numbers the perf gate is argued from.
    desk.log_perf = getenv("DMXDESK_PERF") != NULL;
    // DMXDESK_DRAG=1 moves the master's requested level every loop and
    // repaints, sending nothing: the load of a finger dragging a fader all
    // night, for the perf gate, on a kernel with no uinput to fake a finger.
    app.fake_drag = getenv("DMXDESK_DRAG") != NULL;
    perf_window_init(&desk.paint_ms);
    perf_window_init(&desk.present_ms);
    desk.force_flip = getenv("DMXDESK_FLIP") != NULL;
    // Forced flips run at the panel's own pace, not the loop's.
    app.no_wait = desk.force_flip || app.fake_drag;
    desk.last_report_ms = linux_desk_now(&desk);
    desk.snapshot_pending = 1;
    app.last_link = DESK_CONSOLE_DOWN;
    desk_toggle_queue_init(&app.toggles);

    struct desk_platform platform = {
        .ctx = &desk,
        .now = linux_desk_now,
        .stopping = linux_desk_stopping,
        .wait = linux_desk_wait,
        .power_key = linux_desk_power_key,
        .touches = linux_desk_touches,
        .drop_contacts = linux_desk_drop_contacts,
        .display = linux_desk_display,
        .setup_opened = linux_desk_setup_opened,
        .setup_action = linux_desk_setup_action,
        .service = linux_desk_service,
        .status = linux_desk_status,
        .frame = linux_desk_frame,
    };
    desk_run(&app, &platform);

    if (desk.touch)
        touch_input_free(desk.touch);
    if (desk.touch_fd >= 0)
        close(desk.touch_fd);
    if (desk.power_fd >= 0)
        close(desk.power_fd);
    qlc_session_free(session);
    wifi_join_free(&desk.join);
    if (desk.wpa) {
        int64_t detach_at = loop_trace(NULL, 0);
        wpa_ctrl_close(desk.wpa);
        loop_trace("wpa_ctrl_close", detach_at);
    }
    aw_free(desk.finder);
    desk_power_free(&desk.power);
    desk_fonts_close(&app.fonts);
    free(app.canvas.px);
    present_close(desk.present);
    vc_free(&app.console);
    return 0;
}
