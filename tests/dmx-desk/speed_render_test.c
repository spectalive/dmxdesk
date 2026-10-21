// SOURCES: desk_speed.c desk_speed_paint.c desk_speed_tempo_step_rect.c desk_tap.c speed_factor.c showmap.c showmap_release_to_parse.c showmap_release_to_prune.c vcjson.c canvas.c canvas_blend.c font.c desk_fonts.c desk_paint_keyline.c desk_speed_target_rect.c desk_text_upper.c
// The SPEED page's cards painted: both dials known from the console, one
// change waiting, one noted; then the page before anything is known.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "desk_layout.h"
#include "desk_speed.h"
#include "desk_speed_layout.h"
#include "desk_speed_paint.h"
#include "desk_speed_target_rect.h"
#include "desk_fonts.h"
#include "font.h"

static char *slurp(const char *path, size_t *len) {
    FILE *f = fopen(path, "rb");
    assert(f);
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = malloc((size_t)n + 1);
    assert(buf && fread(buf, 1, (size_t)n, f) == (size_t)n);
    buf[n] = '\0';
    fclose(f);
    *len = (size_t)n;
    return buf;
}

static void save(const struct canvas *c, const char *name) {
    const char *out = getenv("TEST_OUT");
    char path[512];
    snprintf(path, sizeof path, "%s/%s", out ? out : "/tmp", name);
    FILE *f = fopen(path, "wb");
    assert(f);
    fprintf(f, "P6\n%d %d\n255\n", c->w, c->h);
    for (int i = 0; i < c->w * c->h; i++) {
        uint32_t p = c->px[i];
        unsigned char rgb[3] = { (p >> 16) & 255, (p >> 8) & 255, p & 255 };
        fwrite(rgb, 1, 3, f);
    }
    fclose(f);
    printf("wrote %s\n", path);
}

int main(void) {
    size_t map_len, vc_len;
    char *map_text = slurp("show/vibra.desk.json", &map_len);
    char *vc_text = slurp("tests/dmx-desk/fixtures/vc-vibra.json", &vc_len);
    struct show_map map;
    assert(showmap_parse(map_text, map_len, &map) == 0);
    int tempo = -1, movement = -1;
    for (int i = 0; i < map.dials; i++) {
        if (strcmp(map.dial[i].key, "tempo-show") == 0)
            tempo = i;
        if (strcmp(map.dial[i].key, "vel-movimiento") == 0)
            movement = i;
    }
    assert(tempo == 0 && movement == 1);
    const int tempo_widget = map.dial[tempo].widget_id;
    const int movement_widget = map.dial[movement].widget_id;
    struct vc_doc console;
    assert(vc_parse(vc_text, vc_len, &console) == 0);
    struct desk_fonts fonts;
    assert(desk_fonts_open(&fonts, "fonts") == 0);
    struct canvas canvas = { .px = calloc(DESK_W * DESK_H, 4), .w = DESK_W, .h = DESK_H };
    assert(canvas.px);
    for (int i = 0; i < DESK_W * DESK_H; i++)
        canvas.px[i] = DESK_GLASS;

    struct desk_speed s;
    desk_speed_init(&s, &map);
    desk_speed_paint(&canvas, &s, &fonts);
    save(&canvas, "speed-unknown.ppm");

    desk_speed_validate(&s, &console);
    assert(s.dial[0].known && s.dial[0].widget_id == tempo_widget);
    desk_speed_apply(&s, movement_widget, 400, 7, 1000);          // "State updated" on the second
    struct desk_rect up = desk_speed_target_rect(0, SPEED_T_BPM_UP);
    desk_speed_touch_down(&s, up.x + 10, up.y + 10, 1000, 1000);
    desk_speed_touch_up(&s, up.x + 10, up.y + 10, 1000);
    assert(s.dial[0].pending);
    desk_speed_paint(&canvas, &s, &fonts);
    save(&canvas, "speed.ppm");
    // The cards sit side by side in the content only, the ground between
    // them and right of the second.
    for (int i = 0; i < 2; i++)
        assert(canvas.px[(SPEED_CARD_Y + 40) * DESK_W + SPEED_CARD_X(i) + 8] == DESK_PANEL);
    assert(canvas.px[(SPEED_CARD_Y + 200) * DESK_W + SPEED_CARD_X(0) + SPEED_CARD_W + 4] == DESK_GLASS);
    assert(canvas.px[(SPEED_CARD_Y + 40) * DESK_W + 900] == DESK_GLASS);
    assert(canvas.px[20 * DESK_W + 400] == DESK_GLASS);
    // Each target is drawn where it is hit: a live one a tile inside its
    // keyline on the card, a dead one the card inside a dimmer keyline.
    int live = 0;
    for (int i = 0; i < 2; i++)
        for (int t = SPEED_T_TAP; t <= SPEED_T_DOUBLE; t++) {
            struct desk_rect r = desk_speed_target_rect(i, (enum speed_target)t);
            int on = desk_speed_target_enabled(&s, i, (enum speed_target)t);
            live += on;
            assert(canvas.px[(r.y + 4) * DESK_W + r.x + r.w / 2] == (on ? DESK_TILE : DESK_PANEL));
            assert(canvas.px[r.y * DESK_W + r.x + r.w / 2] == (on ? DESK_LINE : DESK_RAISED));
        }
    assert(live > 0);
    printf("speed_render ok\n");
    free(canvas.px);
    vc_free(&console);
    free(map_text);
    free(vc_text);
    return 0;
}
