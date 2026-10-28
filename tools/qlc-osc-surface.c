// Writes the OSC surface file that lets the desk drive QLC+ through QLC+'s
// own OSC input, from the /vc.json of the workspace QLC+ has loaded:
//
//   curl -s http://<master>:9999/vc.json > vc.json
//   qlc-osc-surface --grand-master 266 vc.json > console.osc.json
//   qlc-osc-surface --channels vc.json        # what QLC+ must listen to
//   dmxdesk --osc console.osc.json --host <master> --port 7700 --osc-listen 9000
//
// --prefix sets the addresses' root (default /desk); --no-feedback is for a
// workspace without an OSC feedback patch. The rest is in the README's OSC
// section and core/qlc_osc_item_json.h.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "osc_map_free.h"
#include "osc_map_parse.h"
#include "qlc_osc_channels_write.h"
#include "qlc_osc_surface_json.h"
#include "vcjson.h"

#define VC_MAX_BYTES (4 * 1024 * 1024)

int main(int argc, char **argv) {
    const char *prefix = "/desk", *path = NULL;
    int grand_master = -1, feedback = 1, channels = 0;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--prefix") && i + 1 < argc) prefix = argv[++i];
        else if (!strcmp(argv[i], "--grand-master") && i + 1 < argc) grand_master = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--no-feedback")) feedback = 0;
        else if (!strcmp(argv[i], "--channels")) channels = 1;
        else if (argv[i][0] != '-' && !path) path = argv[i];
        else path = NULL, i = argc;
    }
    if (!path || prefix[0] != '/') {
        fprintf(stderr, "usage: %s [--prefix /desk] [--grand-master ID] [--no-feedback]"
                        " [--channels] VC.JSON\n", argv[0]);
        return 2;
    }
    FILE *f = fopen(path, "rb");
    char *json = malloc(VC_MAX_BYTES);
    size_t len = f && json ? fread(json, 1, VC_MAX_BYTES, f) : 0;
    if (f)
        fclose(f);
    struct vc_doc doc;
    if (!len || len == VC_MAX_BYTES || vc_parse(json, len, &doc) != 0) {
        fprintf(stderr, "%s: not a readable /vc.json\n", path);
        free(json);
        return 1;
    }
    free(json);
    char *surface = qlc_osc_surface_json(&doc, prefix, grand_master, feedback);
    vc_free(&doc);
    struct osc_map map;
    char why[128];
    // The file is parsed back as the desk will read it, so a wrong one, such
    // as a --grand-master that is not a fader, is refused here, not on stage.
    if (!surface || osc_map_parse(surface, strlen(surface), &map, why, sizeof why) != 0) {
        fprintf(stderr, "%s: %s\n", path, surface ? why : "out of memory");
        free(surface);
        return 1;
    }
    int rc = channels ? (qlc_osc_channels_write(&map, stdout) < 0) : (puts(surface) < 0);
    osc_map_free(&map);
    free(surface);
    return rc;
}
