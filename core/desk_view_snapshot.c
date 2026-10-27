#include "desk_view_snapshot.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cjson/cJSON.h>

#include "desk_view.h"
#include "desk_room_summary.h"
#include "desk_pager_label.h"
#include "desk_trajectory_of.h"
#include "speed_factor.h"

char *desk_view_snapshot(const struct desk_model *model,
                         const struct desk_speed *speed,
                         const struct desk_setup *setup) {
    if (!model || !speed || !setup)
        return NULL;
    cJSON *root = cJSON_CreateObject();
    if (!root)
        return NULL;
    cJSON_AddNumberToObject(root, "page", model->page);
    cJSON_AddNumberToObject(root, "bank", model->bank);
    cJSON_AddNumberToObject(root, "link", model->link);
    cJSON_AddBoolToObject(root, "locked", model->locked);
    cJSON_AddBoolToObject(root, "mismatch", model->mismatch);
    cJSON_AddBoolToObject(root, "colorOneShot", model->color_one_shot);
    cJSON_AddNumberToObject(root, "battery", model->battery);
    cJSON_AddBoolToObject(root, "charging", model->charging);
    cJSON_AddNumberToObject(root, "wifiBars", model->wifi_bars);
    cJSON_AddStringToObject(root, "masterName", model->master_name);
    cJSON_AddNumberToObject(root, "speedPage", model->layout.speed_page);
    cJSON_AddNumberToObject(root, "effectsPage", model->effects_page);
    const char *active_mode = "";
    const struct desk_room_summary summary = desk_room_summary(model);
    int haze_off_selected = model->link == DESK_LINK_READY;
    for (int i = 0; i < model->count; i++) {
        const struct desk_control *c = &model->control[i];
        if (c->kind == DESK_CUE && c->role == MAP_ROLE_HAZE &&
            (c->state != DESK_OFF || c->pending))
            haze_off_selected = 0;
        if (c->state != DESK_ON || !c->enabled)
            continue;
        if (c->role == MAP_ROLE_STATE && c->kind == DESK_CUE) active_mode = c->label;
    }
    cJSON_AddStringToObject(root, "activeMode", active_mode);
    cJSON_AddStringToObject(root, "activeColor", summary.colour);
    cJSON_AddStringToObject(root, "activeHaze", summary.smoke);
    cJSON_AddStringToObject(root, "activeEffect", summary.effect);

    cJSON *pages = cJSON_AddArrayToObject(root, "pages");
    for (int i = 0; i < model->layout.pages; i++) {
        cJSON *page = cJSON_CreateObject();
        cJSON_AddNumberToObject(page, "index", i);
        cJSON_AddNumberToObject(page, "tab", model->layout.tab[i]);
        cJSON_AddNumberToObject(page, "banks", model->layout.banks[i]);
        cJSON_AddStringToObject(page, "title", model->layout.title[i]);
        cJSON_AddItemToArray(pages, page);
    }
    cJSON *families = cJSON_AddArrayToObject(root, "families");
    for (int i = 0; i < model->layout.families; i++) {
        cJSON *family = cJSON_CreateObject();
        cJSON_AddNumberToObject(family, "page", model->layout.family_page[i]);
        cJSON_AddNumberToObject(family, "icon", model->layout.family_icon[i]);
        cJSON_AddItemToArray(families, family);
    }
    cJSON *pager = cJSON_AddArrayToObject(root, "pager");
    if (model->page >= 0 && model->page < model->layout.pages) {
        for (int i = 0; i < model->layout.banks[model->page]; i++) {
            struct desk_rect rect = desk_view_pager(i, &model->layout, model->page);
            cJSON *entry = cJSON_CreateObject();
            cJSON_AddNumberToObject(entry, "bank", i);
            cJSON_AddNumberToObject(entry, "x", rect.x);
            cJSON_AddNumberToObject(entry, "y", rect.y);
            cJSON_AddNumberToObject(entry, "w", rect.w);
            cJSON_AddNumberToObject(entry, "h", rect.h);
            cJSON_AddStringToObject(entry, "caption",
                                    desk_pager_label(&model->layout, model->page, i));
            cJSON_AddItemToArray(pager, entry);
        }
    }
    cJSON *headings = cJSON_AddArrayToObject(root, "headings");
    for (int i = 0; i < model->layout.headings; i++) {
        const struct desk_heading *h = &model->layout.heading[i];
        if (h->page != model->page || h->bank != model->bank)
            continue;
        cJSON *heading = cJSON_CreateObject();
        cJSON_AddNumberToObject(heading, "x", h->x);
        cJSON_AddNumberToObject(heading, "y", h->y);
        cJSON_AddNumberToObject(heading, "w", h->w);
        cJSON_AddNumberToObject(heading, "h", h->h);
        cJSON_AddStringToObject(heading, "text", h->text);
        cJSON_AddNumberToObject(heading, "part", h->part);
        cJSON_AddNumberToObject(heading, "parts", h->parts);
        cJSON_AddItemToArray(headings, heading);
    }
    cJSON *controls = cJSON_AddArrayToObject(root, "controls");
    for (int i = 0; i < model->layout.placements; i++) {
        const struct desk_placement *p = &model->layout.placement[i];
        if (p->page != model->page || p->bank != model->bank ||
            p->control < 0 || p->control >= model->count)
            continue;
        const struct desk_control *c = &model->control[p->control];
        cJSON *control = cJSON_CreateObject();
        cJSON_AddNumberToObject(control, "index", p->control);
        cJSON_AddNumberToObject(control, "x", p->x);
        cJSON_AddNumberToObject(control, "y", p->y);
        cJSON_AddNumberToObject(control, "w", p->w);
        cJSON_AddNumberToObject(control, "h", p->h);
        cJSON_AddNumberToObject(control, "tile", p->tile);
        cJSON_AddNumberToObject(control, "trajectory",
                                p->tile == TILE_TRAJECTORY ? desk_trajectory_of(c->label) : -1);
        cJSON_AddNumberToObject(control, "kind", c->kind);
        cJSON_AddNumberToObject(control, "role", c->role);
        cJSON_AddStringToObject(control, "label", c->label);
        cJSON_AddStringToObject(control, "detail", c->detail);
        cJSON_AddStringToObject(control, "icon", c->icon);
        cJSON_AddStringToObject(control, "reason", c->reason);
        cJSON_AddNumberToObject(control, "state", c->state);
        cJSON_AddNumberToObject(control, "level", c->level);
        cJSON_AddNumberToObject(control, "requestedLevel", c->requested_level);
        cJSON_AddNumberToObject(control, "holdProgress", c->hold_progress);
        cJSON_AddBoolToObject(control, "enabled", c->enabled);
        cJSON_AddBoolToObject(control, "selected",
                              c->kind == DESK_HAZE_OFF ? haze_off_selected :
                              c->state == DESK_ON && c->enabled);
        cJSON_AddBoolToObject(control, "pressed", c->pressed);
        cJSON_AddBoolToObject(control, "pending", c->pending);
        cJSON *swatches = cJSON_AddArrayToObject(control, "swatches");
        for (int j = 0; j < c->swatches && j < DESK_MAX_SWATCHES; j++) {
            char color[8];
            snprintf(color, sizeof color, "#%06x", c->swatch[j] & 0xffffffu);
            cJSON_AddItemToArray(swatches, cJSON_CreateString(color));
        }
        cJSON_AddItemToArray(controls, control);
    }
    cJSON *dials = cJSON_AddArrayToObject(root, "dials");
    for (int i = 0; i < speed->dials; i++) {
        const struct desk_dial *d = &speed->dial[i];
        cJSON *dial = cJSON_CreateObject();
        cJSON_AddStringToObject(dial, "caption", d->caption);
        cJSON_AddNumberToObject(dial, "bpm", d->base_ms > 0
                                ? (60000 + d->base_ms / 2) / d->base_ms : 0);
        cJSON_AddNumberToObject(dial, "baseMs", d->base_ms);
        cJSON_AddNumberToObject(dial, "factor", d->factor);
        cJSON_AddStringToObject(dial, "factorName", speed_factor_name(d->factor));
        cJSON_AddBoolToObject(dial, "known", d->known);
        cJSON_AddBoolToObject(dial, "enabled", d->enabled);
        cJSON_AddBoolToObject(dial, "pending", d->pending);
        cJSON_AddStringToObject(dial, "note", d->note);
        cJSON_AddItemToArray(dials, dial);
    }
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON_AddBoolToObject(settings, "open", setup->open);
    cJSON_AddBoolToObject(settings, "wifiAvailable", setup->wifi_available);
    cJSON_AddStringToObject(settings, "ssid", setup->ssid);
    cJSON_AddStringToObject(settings, "wifiState", setup->wifi_state);
    cJSON_AddStringToObject(settings, "wifiBusy", setup->wifi_busy);
    cJSON_AddStringToObject(settings, "wifiNote", setup->wifi_note);
    cJSON_AddStringToObject(settings, "master", setup->master);
    cJSON_AddNumberToObject(settings, "port", setup->port);
    cJSON_AddStringToObject(settings, "masterBusy", setup->master_busy);
    cJSON_AddStringToObject(settings, "masterNote", setup->master_note);
    cJSON_AddNumberToObject(settings, "brightness", setup->brightness);
    cJSON_AddNumberToObject(settings, "brightnessMax", setup->brightness_max);
    cJSON_AddBoolToObject(settings, "powerAware", setup->power_aware);
    cJSON_AddBoolToObject(settings, "keyboardOpen", setup->kb.open);
    cJSON_AddStringToObject(settings, "keyboardTitle", setup->kb.title);
    cJSON_AddNumberToObject(settings, "keyboardLength", setup->kb.len);
    cJSON_AddBoolToObject(settings, "keyboardMasked", setup->kb.masked);
    cJSON_AddStringToObject(settings, "keyboardText",
                            setup->kb_purpose == KB_FOR_HOST ? setup->kb.text : "");
    cJSON *keys = cJSON_AddArrayToObject(settings, "keyboardKeys");
    if (setup->kb.open) {
        struct kb_key visible[KB_MAX_KEYS];
        int count = keyboard_keys(&setup->kb, visible, KB_MAX_KEYS);
        for (int i = 0; i < count; i++) {
            cJSON *key = cJSON_CreateObject();
            cJSON_AddNumberToObject(key, "x", visible[i].x);
            cJSON_AddNumberToObject(key, "y", visible[i].y);
            cJSON_AddNumberToObject(key, "w", visible[i].w);
            cJSON_AddNumberToObject(key, "h", visible[i].h);
            cJSON_AddStringToObject(key, "label", visible[i].label);
            cJSON_AddBoolToObject(key, "enabled", visible[i].enabled);
            cJSON_AddItemToArray(keys, key);
        }
    }
    cJSON_AddBoolToObject(settings, "confirmOpen", setup->confirm_open);
    cJSON_AddStringToObject(settings, "confirmSsid", setup->pending_ssid);
    cJSON_AddBoolToObject(settings, "confirmKnown", setup->confirm_known);
    cJSON_AddBoolToObject(settings, "confirmOpenNetwork", setup->confirm_is_open_network);
    cJSON_AddNumberToObject(settings, "scanPage", setup->scan_page);
    cJSON_AddNumberToObject(settings, "foundPage", setup->found_page);
    cJSON *networks = cJSON_AddArrayToObject(settings, "networks");
    for (int i = 0; i < setup->scan.count && i < WIFI_SCAN_MAX; i++) {
        cJSON *network = cJSON_CreateObject();
        cJSON_AddStringToObject(network, "ssid", setup->scan.network[i].ssid);
        cJSON_AddNumberToObject(network, "level", setup->scan.network[i].level_dbm);
        cJSON_AddNumberToObject(network, "security", setup->scan.network[i].security);
        cJSON_AddBoolToObject(network, "known", setup->known[i]);
        cJSON_AddItemToArray(networks, network);
    }
    cJSON *found = cJSON_AddArrayToObject(settings, "found");
    for (int i = 0; i < setup->found_count && i < SETUP_FOUND_MAX; i++) {
        cJSON *master = cJSON_CreateObject();
        cJSON_AddStringToObject(master, "host", setup->found[i]);
        cJSON_AddNumberToObject(master, "port", setup->found_port[i]);
        cJSON_AddItemToArray(found, master);
    }
    char *json = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    if (json && strlen(json) > 131072) {
        free(json);
        return NULL;
    }
    return json;
}
