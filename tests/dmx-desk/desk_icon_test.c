// SOURCES: icon_glyph.c showmap.c showmap_release_to_parse.c showmap_release_to_prune.c showmap_validate.c desk_model.c desk_master_level_at.c desk_paint.c desk_master_track.c desk_pager_caption.c desk_pager_bank_caption.c icon.c desk_caption.c desk_view.c desk_view_pager.c desk_pager_label.c desk_layout_resolve.c desk_show_layout.c vcjson.c canvas.c canvas_blend.c font.c desk_fonts.c desk_paint_keyline.c desk_paint_text_centred.c desk_paint_tabs.c desk_paint_families.c desk_tab_page.c desk_tab_title.c desk_view_family.c desk_family_icon.c desk_show_slot.c desk_show_place.c desk_show_panel.c desk_show_section.c desk_show_accent_tile.c desk_show_live_page.c desk_show_is_rig.c desk_text_upper.c desk_colour_dim.c desk_panel_holds.c desk_panel_running.c desk_rig_name.c desk_paint_room.c desk_paint_room_status.c desk_paint_text_at.c desk_pick_numbered.c desk_caption_number.c desk_trajectory_of.c desk_paint_number.c desk_paint_trajectory.c desk_paint_tile_base.c icon_paint_large.c desk_show_holds.c desk_show_haze.c desk_show_rig.c desk_damage_running.c desk_paint_hold.c desk_paint_selected.c desk_paint_selected_mark.c icon_paint_mid.c desk_paint_room_picture.c icon_paint_hero.c
// The map's per-control "icon" (2026-10-19, qlctool deskmap splits the glyph
// off the console caption): every glyph the shipped map uses has a mark, the
// parser keeps the field and shrugs off one it cannot hold, and the painter
// draws the mark on exactly the tiles that carry a known glyph. A tile whose
// glyph is missing or unknown paints as if the map had no icon field at all.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cjson/cJSON.h>

#include "canvas.h"
#include "desk_layout.h"
#include "desk_layout_resolve.h"
#include "desk_model.h"
#include "desk_paint.h"
#include "icon_glyph.h"
#include "showmap.h"
#include "showmap_validate.h"
#include "vcjson.h"

static char *slurp(const char *path, size_t *len) {
    FILE *f = fopen(path, "rb");
    assert(f);
    assert(fseek(f, 0, SEEK_END) == 0);
    long size = ftell(f);
    assert(size > 0);
    assert(fseek(f, 0, SEEK_SET) == 0);
    char *buf = malloc((size_t)size + 1);
    assert(buf);
    assert(fread(buf, 1, (size_t)size, f) == (size_t)size);
    fclose(f);
    buf[size] = '\0';
    *len = (size_t)size;
    return buf;
}

static char *edited(const char *text, const char *from, const char *to) {
    const char *at = strstr(text, from);
    assert(at);
    size_t before = (size_t)(at - text), flen = strlen(from), tlen = strlen(to);
    char *out = malloc(strlen(text) - flen + tlen + 1);
    assert(out);
    memcpy(out, text, before);
    memcpy(out + before, to, tlen);
    strcpy(out + before + tlen, at + flen);
    return out;
}

static int control_keyed(const struct show_map *map, const char *key) {
    for (int i = 0; i < map->count; i++)
        if (strcmp(map->control[i].key, key) == 0)
            return i;
    return -1;
}

static int differs(const struct canvas *a, const struct canvas *b, const struct desk_placement *p) {
    for (int y = p->y; y < p->y + p->h; y++)
        for (int x = p->x; x < p->x + p->w; x++)
            if (a->px[y * a->w + x] != b->px[y * b->w + x])
                return 1;
    return 0;
}

static struct show_map map;
static struct desk_model marked, plain;

int main(void) {
    // The lookup: the glyphs, the emoji selector, the unknown and the empty.
    assert(icon_glyph("\xe2\x9a\xa1") == ICON_BOLT);
    assert(icon_glyph("\xe2\x98\x81") == ICON_CLOUD);
    assert(icon_glyph("\xe2\x98\x81\xef\xb8\x8f") == ICON_CLOUD);
    assert(icon_glyph("\xf0\x9f\x8e\xa8") == ICON_PALETTE);
    assert(icon_glyph("\xe2\x98\x83") == -1);
    assert(icon_glyph("A") == -1);
    assert(icon_glyph("") == -1 && icon_glyph(NULL) == -1);
    for (size_t i = 0; i < sizeof ICON_GLYPHS / sizeof ICON_GLYPHS[0]; i++)
        assert(icon_glyph(ICON_GLYPHS[i].glyph) == (int)ICON_GLYPHS[i].id);

    // The parser reports what the file says, read here with cJSON.
    size_t len;
    char *text = slurp("show/vibra.desk.json", &len);
    assert(showmap_parse(text, len, &map) == 0);
    cJSON *root = cJSON_Parse(text);
    assert(root);
    const cJSON *controls = cJSON_GetObjectItemCaseSensitive(root, "controls");
    int with_icon = 0;
    for (int i = 0; i < map.count; i++) {
        const cJSON *c = cJSON_GetObjectItemCaseSensitive(controls, map.control[i].key);
        const cJSON *icon = cJSON_GetObjectItemCaseSensitive(c, "icon");
        const char *want = cJSON_IsString(icon) ? icon->valuestring : "";
        assert(strcmp(map.control[i].icon, want) == 0);
        if (want[0]) {
            with_icon++;
            // Every glyph the show uses is one the desk can draw.
            assert(icon_glyph(want) >= 0);
        }
    }
    cJSON_Delete(root);
    assert(with_icon > 100);
    printf("controls %d, with an icon %d\n", map.count, with_icon);

    // An unknown glyph is kept and draws nothing; one too long for the field
    // is dropped; neither refuses the map.
    char *unknown = edited(text, "\"icon\": \"\xe2\x96\xb6\"", "\"icon\": \"\xe2\x98\x83\"");
    assert(showmap_parse(unknown, strlen(unknown), &map) == 0);
    assert(strcmp(map.control[control_keyed(&map, "auto")].icon, "\xe2\x98\x83") == 0);
    free(unknown);
    char *long_icon = edited(text, "\"icon\": \"\xe2\x96\xb6\"", "\"icon\": \"abcdefghijklmnopqrstuvwxyz\"");
    assert(showmap_parse(long_icon, strlen(long_icon), &map) == 0);
    assert(map.control[control_keyed(&map, "auto")].icon[0] == '\0');
    free(long_icon);
    char *none = edited(text, "\"icon\": \"\xe2\x96\xb6\",", "");
    assert(showmap_parse(none, strlen(none), &map) == 0);
    assert(map.control[control_keyed(&map, "auto")].icon[0] == '\0');
    free(none);

    // The painter: the same desk with and without its icons, every page and
    // bank. A tile differs exactly when it carries a known glyph and is one
    // that wears a mark (not a colour square, a number or a trajectory); the bolt stays on a hit without one.
    assert(showmap_parse(text, len, &map) == 0);
    free(text);
    char *json = slurp("tests/dmx-desk/fixtures/vc-vibra.json", &len);
    struct vc_doc console;
    assert(vc_parse(json, len, &console) == 0);
    free(json);
    assert(showmap_build(&marked, &map, &console) > 100);
    struct desk_layout layout;
    assert(desk_layout_resolve(&map, map.count, map.count + 1, map.count + 2, map.count + 3, &layout) == 0);
    desk_set_layout(&marked, &layout);
    desk_set_link(&marked, DESK_LINK_READY);
    plain = marked;
    for (int i = 0; i < plain.count; i++)
        plain.control[i].icon[0] = '\0';
    // An unknown glyph paints like none.
    int auto_index = control_keyed(&map, "auto");
    snprintf(plain.control[auto_index].icon, sizeof plain.control[auto_index].icon, "\xe2\x98\x83");

    struct desk_fonts fonts;
    assert(desk_fonts_open(&fonts, "fonts") == 0);
    struct canvas a = { .px = calloc(DESK_W * DESK_H, 4), .w = DESK_W, .h = DESK_H };
    struct canvas b = { .px = calloc(DESK_W * DESK_H, 4), .w = DESK_W, .h = DESK_H };
    assert(a.px && b.px);
    int drawn = 0, kept = 0;
    for (int page = 0; page < layout.pages; page++)
        for (int bank = 0; bank < layout.banks[page]; bank++) {
            desk_set_view(&marked, page, bank);
            desk_set_view(&plain, page, bank);
            desk_paint(&a, &marked, &fonts);
            desk_paint(&b, &plain, &fonts);
            for (int i = 0; i < layout.placements; i++) {
                const struct desk_placement *p = &layout.placement[i];
                if (p->page != page || p->bank != bank || p->control >= map.count)
                    continue;
                const struct desk_control *ctl = &marked.control[p->control];
                // A number or a trajectory stands in the mark's place.
                int wears = icon_glyph(ctl->icon) >= 0 && p->tile != TILE_MINI &&
                            p->tile != TILE_NUMBER && p->tile != TILE_TRAJECTORY &&
                            !(p->tile == TILE_SWATCH && ctl->swatches > 0) &&
                            // The flash's mark is the bolt a hit already wears.
                            !((p->tile == TILE_HOLD || p->tile == TILE_FOG) &&
                              icon_glyph(ctl->icon) == ICON_BOLT);
                // The compact state row gives long captions room before a
                // decorative mark; BLANCO TOTAL is one such state.
                if ((p->tile == TILE_STATE && p->h <= DESK_STATE_H) ||
                    p->tile == TILE_SEGMENT) {
                    drawn += differs(&a, &b, p);
                    kept += !differs(&a, &b, p);
                    continue;
                }
                if (differs(&a, &b, p) != wears) {
                    fprintf(stderr, "%s on page %d: wears %d\n", ctl->label, page, wears);
                    assert(0);
                }
                drawn += wears;
                kept += !wears;
            }
        }
    printf("tiles with a mark %d, unchanged %d\n", drawn, kept);
    // Numbers and trajectories (56 picks) stand where their glyph was.
    assert(drawn > 40 && kept > 0);

    // The look: SHOW with its marks, beside desk.ppm.
    desk_set_view(&marked, 0, 0);
    desk_paint(&a, &marked, &fonts);
    const char *out = getenv("TEST_OUT");
    char path[512];
    snprintf(path, sizeof path, "%s/desk-icons.ppm", out ? out : "/tmp");
    FILE *f = fopen(path, "wb");
    assert(f);
    fprintf(f, "P6\n%d %d\n255\n", a.w, a.h);
    for (int i = 0; i < a.w * a.h; i++) {
        uint32_t px = a.px[i];
        unsigned char rgb[3] = { (px >> 16) & 0xff, (px >> 8) & 0xff, px & 0xff };
        fwrite(rgb, 1, 3, f);
    }
    fclose(f);
    free(a.px);
    free(b.px);
    desk_fonts_close(&fonts);
    vc_free(&console);
    printf("desk_icon ok\n");
    return 0;
}
