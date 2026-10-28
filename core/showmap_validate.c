#include "showmap_validate.h"

#include <stdio.h>
#include <string.h>
#include "desk_surface_find.h"
#include "desk_surface_is_button.h"
#include "showmap_check_priority.h"


static void disable(struct desk_control *c, const char *reason) {
    c->enabled = 0;
    snprintf(c->reason, sizeof c->reason, "%s", reason);
}

// Whether the widget sits inside the frame, at any depth: a pick in an inner
// multipage frame still belongs to the solo frame around it.
static int under_frame(const struct desk_surface *doc, const struct desk_surface_item *w, int frame_id) {
    int cur = w->group_id;
    for (int depth = 0; depth < 16 && cur >= 0; depth++) {
        if (cur == frame_id)
            return 1;
        const struct desk_surface_item *p = desk_surface_find(doc, cur);
        if (!p || p->group_id == cur)
            break;
        cur = p->group_id;
    }
    return 0;
}

// A cue may be pressed only when the console really has that widget, it really
// is a button, it really toggles, it really drives the function the map
// claims, and it sits in the solo frame the map says it does. Anything else
// and the tile is drawn but dead.
// A hit fired while the finger is down: its widget must be a Flash button
// of this show. Fog stays on the Mac until a finite burst is proven there:
// a tablet's cap cannot bound an output after the link is lost.
static const struct desk_surface_item *check_hold(struct desk_control *c,
                                          const struct map_control *m,
                                          const struct desk_surface *doc) {
    const struct desk_surface_item *w = desk_surface_find(doc, m->widget_id);
    if (!w) {
        disable(c, "not in this show");
        return NULL;
    }
    if (w->kind != SURFACE_FLASH) {
        disable(c, "not a flash button");
        return NULL;
    }
    if (m->function_id >= 0 && w->function_id != m->function_id) {
        disable(c, "drives another cue");
        return NULL;
    }
    if (w->disabled) {
        disable(c, "disabled on the master");
        return NULL;
    }
    c->function_id = w->function_id;
    c->enabled = 1;
    c->reason[0] = '\0';
    return w;
}

// Fog and strobes stay on the Mac: a tablet's cap cannot bound an output
// after the link is lost, and neither may be left running unattended.
static int is_fog(const struct map_control *m) {
    return strncmp(m->key, "humo", 4) == 0 || strncmp(m->key, "strobo", 6) == 0;
}

static const struct desk_surface_item *check_cue(struct desk_control *c,
                                         const struct map_control *m,
                                         const struct desk_surface *doc) {
    const struct desk_surface_item *w = desk_surface_find(doc, m->widget_id);
    if (!w) {
        disable(c, "not in this show");
        return NULL;
    }
    if (!desk_surface_is_button(w->kind)) {
        disable(c, "not a button");
        return NULL;
    }
    if (w->kind != SURFACE_TOGGLE) {
        // Flash is the one that cannot be made safe over a network.
        disable(c, w->kind == SURFACE_FLASH ? "held, not safe here"
                                              : "wrong button action");
        return NULL;
    }
    if (m->function_id >= 0 && w->function_id != m->function_id) {
        disable(c, "drives another cue");
        return NULL;
    }
    if (m->solo_id >= 0 && !under_frame(doc, w, m->solo_id)) {
        disable(c, "outside its frame");
        return NULL;
    }
    if (w->disabled) {
        disable(c, "disabled on the master");
        return NULL;
    }
    c->function_id = w->function_id;
    c->enabled = 1;
    c->reason[0] = '\0';
    return w;
}

// The console must be the one the map was generated for: the same schema on
// this side and the same QLC+ line on the master's. A mismatch disables the
// whole desk, because a map read against another show cannot be reasoned
// about, only misread.
int showmap_mismatch(const struct show_map *map, const struct desk_surface *doc) {
    if (map->schema != 2)
        return 1;
    if (doc->version[0] && strcmp(doc->version, map->qlc_version) != 0)
        return 1;
    return 0;
}

int showmap_build(struct desk_model *model, const struct show_map *map,
                  const struct desk_surface *doc) {
    desk_init(model);
    snprintf(model->master_name, sizeof model->master_name, "%s", map->key);
    int wrong_show = showmap_mismatch(map, doc);
    model->mismatch = wrong_show;

    int enabled = 0;
    for (int i = 0; i < map->count; i++) {
        const struct map_control *m = &map->control[i];
        struct desk_control c;
        memset(&c, 0, sizeof c);
        c.kind = DESK_CUE;
        snprintf(c.label, sizeof c.label, "%s", m->caption);
        snprintf(c.detail, sizeof c.detail, "%s", m->detail);
        snprintf(c.icon, sizeof c.icon, "%s", m->icon);
        c.widget_id = m->widget_id;
        c.function_id = m->function_id;
        c.priority_widget_id = -1;
        c.page = m->page;
        c.section = m->section;
        c.role = m->role;
        c.shot_widget_id = -1;
        for (int s = 0; s < m->swatches; s++)
            c.swatch[s] = 0xFF000000u | m->swatch[s];
        c.swatches = m->swatches;

        const struct desk_surface_item *widget = NULL;
        if (wrong_show)
            disable(&c, "show mismatch");
        else if (m->role == MAP_ROLE_BURST) {
            c.kind = DESK_BURST;
            c.burst_ms = m->burst_ms;
            widget = check_cue(&c, m, doc);
            if (widget) {
                const char *priority_error = showmap_check_priority(m, doc);
                if (priority_error)
                    disable(&c, priority_error);
                else
                    c.priority_widget_id = m->priority_widget_id;
            }
        } else if (m->held && !is_fog(m)) {
            c.kind = DESK_HOLD;
            widget = check_hold(&c, m, doc);
        } else if (!m->enabled)
            disable(&c, m->reason[0] ? m->reason : "held on the Mac");
        else
            widget = check_cue(&c, m, doc);
        c.hold_index = -1;

        int index = desk_add(model, &c);
        if (index < 0) {
            fprintf(stderr, "desk: more controls than the model holds\n");
            break;
        }
        // The console's document carries each widget's state as it was when
        // the desk fetched it. That is the only snapshot there is: opening the
        // socket sends nothing, so without this every tile would start
        // unknown and stay unknown until someone touched the show.
        if (widget)
            model->control[index].state = widget->on ? DESK_ON : DESK_OFF;
        if (!c.enabled && !wrong_show)
            fprintf(stderr, "desk: %s: %s\n", c.label, c.reason);
        enabled += c.enabled;
    }

    struct desk_control master;
    memset(&master, 0, sizeof master);
    master.kind = DESK_MASTER;
    snprintf(master.label, sizeof master.label, "Master");
    master.widget_id = map->grand_master_widget;
    master.function_id = -1;
    master.page = -1;
    master.enabled = !wrong_show;
    if (wrong_show)
        disable(&master, "show mismatch");
    // The console's own word on the level: the snapshot carries the slider,
    // so the fader is known from the first frame rather than from the first
    // time somebody moves it on the Mac.
    const struct desk_surface_item *gm = wrong_show ? NULL : desk_surface_find(doc, map->grand_master_widget);
    int gm_known = gm && gm->kind == SURFACE_LEVEL;
    master.level = gm_known ? gm->level : 0;
    master.requested_level = master.level;
    // A fader the console will not move is not offered, as for every other
    // control: QLC+ ignores OSC input to a widget on a hidden frame page, and
    // its OSC surface file declares such a widget disabled (2026-10-28).
    if (gm_known && gm->disabled)
        disable(&master, "disabled on the master");
    int master_index = desk_add(model, &master);
    if (master_index >= 0) {
        enabled += master.enabled;
        // desk_add starts every control unknown; the console's word comes after.
        if (gm_known)
            model->control[master_index].state = DESK_ON;
    }

    // SHOW's two pseudo controls, always present after the panic button so
    // their indices are known: ambient OFF, and the tempo card for the first
    // dial. Each is disabled when the show has nothing for it.
    struct desk_control off;
    memset(&off, 0, sizeof off);
    off.kind = DESK_HAZE_OFF;
    snprintf(off.label, sizeof off.label, "OFF");
    off.widget_id = -1;
    off.function_id = -1;
    off.page = -1;
    off.role = MAP_ROLE_HAZE;
    off.enabled = !wrong_show;
    if (wrong_show)
        disable(&off, "show mismatch");
    struct desk_control tempo;
    memset(&tempo, 0, sizeof tempo);
    tempo.kind = DESK_TEMPO;
    snprintf(tempo.label, sizeof tempo.label, "%s", map->dials > 0 ? map->dial[0].caption : "Tempo");
    tempo.widget_id = map->dials > 0 ? map->dial[0].widget_id : -1;
    tempo.function_id = -1;
    tempo.page = -1;
    tempo.enabled = !wrong_show && map->dials > 0;
    if (!tempo.enabled)
        disable(&tempo, wrong_show ? "show mismatch" : "no dial in this show");

    // The panic button: the console's own StopAll, which stops every function
    // whatever started it and never lies about a state of its own.
    struct desk_control stop;
    memset(&stop, 0, sizeof stop);
    stop.kind = DESK_STOP_ALL;
    snprintf(stop.label, sizeof stop.label, "PARAR TODO");
    if (map->stop_all_fade_ms > 0)
        snprintf(stop.detail, sizeof stop.detail, "%d.%d s fade",
                 map->stop_all_fade_ms / 1000, (map->stop_all_fade_ms % 1000) / 100);
    stop.widget_id = map->stop_all_widget;
    stop.function_id = -1;
    stop.page = -1;
    const struct desk_surface_item *w = map->stop_all_widget >= 0 ? desk_surface_find(doc, map->stop_all_widget) : NULL;
    if (wrong_show)
        disable(&stop, "show mismatch");
    else if (!w)
        disable(&stop, "not in this show");
    else if (w->kind != SURFACE_STOP_ALL)
        disable(&stop, "not a stop-all button");
    else
        stop.enabled = 1;
    if (desk_add(model, &stop) >= 0)
        enabled += stop.enabled;
    struct desk_control color_mode;
    memset(&color_mode, 0, sizeof color_mode);
    color_mode.kind = DESK_COLOR_MODE;
    snprintf(color_mode.label, sizeof color_mode.label, "GOLPES DE COLOR");
    color_mode.widget_id = -1;
    color_mode.function_id = -1;
    color_mode.shot_widget_id = -1;
    color_mode.page = -1;
    color_mode.enabled = !wrong_show;
    for (int i = 0; i < map->count; i++) {
        if (strncmp(map->control[i].key, "rig-", 4) != 0 || map->control[i].swatches != 1)
            continue;
        int found = 0;
        for (int j = 0; j < map->count; j++) {
            if (strcmp(map->control[i].key + 4, map->control[j].key) != 0 ||
                model->control[j].kind != DESK_BURST || !model->control[j].enabled)
                continue;
            model->control[i].shot_widget_id = model->control[j].widget_id;
            found = 1;
            break;
        }
        if (!found)
            color_mode.enabled = 0;
    }
    if (!color_mode.enabled)
        disable(&color_mode, wrong_show ? "show mismatch" : "colour hits unavailable");
    if (desk_add(model, &off) < 0 || desk_add(model, &tempo) < 0 ||
        desk_add(model, &color_mode) < 0)
        fprintf(stderr, "desk: no room for SHOW's own controls\n");
    else {
        model->control[DESK_COLOR_MODE_INDEX(map)].state = DESK_OFF;
        enabled += off.enabled + tempo.enabled + color_mode.enabled;
    }
    // A separate control gives the white swatch its own touch target while
    // keeping BLANCO TOTAL in the state row. In one-shot mode it uses the
    // validated, bounded white burst just like the other colour swatches.
    struct desk_control white;
    memset(&white, 0, sizeof white);
    white.kind = DESK_CUE;
    white.widget_id = -1;
    white.function_id = -1;
    white.shot_widget_id = -1;
    white.page = -1;
    int white_source = -1;
    for (int i = 0; i < map->count; i++)
        if (strcmp(map->control[i].key, "blanco-total") == 0)
            white_source = i;
    if (white_source >= 0) {
        white = model->control[white_source];
        snprintf(white.label, sizeof white.label, "BLANCO");
        for (int i = 0; i < map->count; i++)
            if (strcmp(map->control[i].key, "blanco") == 0 &&
                model->control[i].kind == DESK_BURST && model->control[i].enabled)
                white.shot_widget_id = model->control[i].widget_id;
        if (white.shot_widget_id < 0 && model->control[DESK_COLOR_MODE_INDEX(map)].enabled) {
            disable(&model->control[DESK_COLOR_MODE_INDEX(map)], "colour hits unavailable");
            enabled--;
        }
    } else
        disable(&white, "not in this show");
    int white_index = desk_add(model, &white);
    if (white_index >= 0) {
        model->control[white_index].state = white_source >= 0 ?
            model->control[white_source].state : DESK_UNKNOWN;
        enabled += white.enabled;
    }
    return enabled;
}
