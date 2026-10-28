// SOURCES: desk_view_snapshot.c keyboard.c desk_view_pager.c desk_pager_label.c desk_pager_bank_caption.c desk_pager_caption.c canvas.c canvas_blend.c font.c desk_trajectory_of.c desk_room_summary.c desk_rig_name.c speed_factor.c
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <cjson/cJSON.h>

#include "desk_view_snapshot.h"

int main(void) {
    struct desk_model model = {0};
    model.page = 1;
    model.bank = 0;
    model.link = DESK_LINK_READY;
    model.color_one_shot = 1;
    model.count = 4;
    model.control[0].kind = DESK_CUE;
    model.control[0].role = MAP_ROLE_PICK;
    model.control[0].state = DESK_ON;
    model.control[0].enabled = 1;
    model.control[0].swatches = 1;
    model.control[0].swatch[0] = 0x00eb4178;
    strcpy(model.control[0].label, "Pink");
    model.control[1].kind = DESK_MASTER;
    model.control[1].role = MAP_ROLE_STATE;
    model.control[1].state = DESK_ON;
    model.control[1].enabled = 1;
    strcpy(model.control[1].label, "Master");
    model.control[2].kind = DESK_HAZE_OFF;
    model.control[2].enabled = 1;
    strcpy(model.control[2].label, "OFF");
    model.control[3].kind = DESK_CUE;
    model.control[3].role = MAP_ROLE_HAZE;
    model.control[3].state = DESK_OFF;
    model.control[3].enabled = 1;
    strcpy(model.control[3].label, "Every minute");
    model.layout.pages = 2;
    model.layout.banks[1] = 2;
    model.layout.headings = 1;
    model.layout.heading[0].page = 1;
    model.layout.heading[0].bank = 0;
    strcpy(model.layout.heading[0].text, "COLOR");
    model.layout.placements = 3;
    model.layout.placement[0] = (struct desk_placement){.control=0,.page=1,.bank=0,
                                                        .x=40,.y=70,.w=90,.h=88,
                                                        .tile=TILE_MINI};
    model.layout.placement[1] = (struct desk_placement){.control=2,.page=1,.bank=0,
                                                        .x=140,.y=70,.w=60,.h=88,
                                                        .tile=TILE_SEGMENT};
    model.layout.placement[2] = (struct desk_placement){.control=3,.page=1,.bank=0,
                                                        .x=208,.y=70,.w=90,.h=88,
                                                        .tile=TILE_SEGMENT};
    struct desk_speed speed = {0};
    speed.dials = 1;
    speed.dial[0].base_ms = 500;
    speed.dial[0].known = 1;
    struct desk_setup setup = {0};
    strcpy(setup.confirm_psk, "TOPSECRET");
    strcpy(setup.kb.text, "ANOTHERSECRET");
    char *line = desk_view_snapshot(&model, &speed, &setup);
    assert(line);
    assert(!strstr(line, "TOPSECRET") && !strstr(line, "ANOTHERSECRET"));
    cJSON *root = cJSON_Parse(line);
    assert(root);
    assert(cJSON_GetObjectItem(root, "page")->valueint == 1);
    assert(cJSON_IsTrue(cJSON_GetObjectItem(root, "colorOneShot")));
    assert(strcmp(cJSON_GetObjectItem(root, "activeMode")->valuestring, "") == 0);
    assert(strcmp(cJSON_GetObjectItem(root, "activeColor")->valuestring, "Pink") == 0);
    assert(strcmp(cJSON_GetObjectItem(cJSON_GetArrayItem(cJSON_GetObjectItem(root, "pager"), 0),
                                      "caption")->valuestring, "COLOR") == 0);
    cJSON *controls = cJSON_GetObjectItem(root, "controls");
    assert(cJSON_GetArraySize(controls) == 3);
    cJSON *control = cJSON_GetArrayItem(controls, 0);
    assert(strcmp(cJSON_GetObjectItem(control, "label")->valuestring, "Pink") == 0);
    assert(cJSON_GetObjectItem(control, "x")->valueint == 40);
    assert(cJSON_IsTrue(cJSON_GetObjectItem(cJSON_GetArrayItem(controls, 1), "selected")));
    assert(strcmp(cJSON_GetArrayItem(cJSON_GetObjectItem(control, "swatches"), 0)->valuestring,
                  "#eb4178") == 0);
    assert(cJSON_GetObjectItem(cJSON_GetArrayItem(cJSON_GetObjectItem(root,"dials"),0),
                               "bpm")->valueint == 120);
    assert(strcmp(cJSON_GetObjectItem(cJSON_GetArrayItem(cJSON_GetObjectItem(root,"dials"),0),
                                      "factorName")->valuestring, "None") == 0);
    cJSON_Delete(root);
    free(line);
    model.control[3].state = DESK_ON;
    line = desk_view_snapshot(&model, &speed, &setup);
    assert(line);
    root = cJSON_Parse(line);
    assert(root);
    controls = cJSON_GetObjectItem(root, "controls");
    assert(cJSON_IsFalse(cJSON_GetObjectItem(cJSON_GetArrayItem(controls, 1), "selected")));
    assert(strcmp(cJSON_GetObjectItem(root, "activeHaze")->valuestring,
                  "Every minute") == 0);
    cJSON_Delete(root);
    free(line);
    strcpy(model.control[0].label, "Circulo Simultaneo");
    model.layout.placement[0].tile = TILE_TRAJECTORY;
    line = desk_view_snapshot(&model, &speed, &setup);
    assert(line);
    root = cJSON_Parse(line);
    assert(root);
    controls = cJSON_GetObjectItem(root, "controls");
    assert(cJSON_GetObjectItem(cJSON_GetArrayItem(controls, 0), "trajectory")->valueint == 1);
    cJSON_Delete(root);
    free(line);
    model.mismatch = 1;
    model.control[0].enabled = 0;
    line = desk_view_snapshot(&model, &speed, &setup);
    assert(line);
    root = cJSON_Parse(line);
    assert(root);
    controls = cJSON_GetObjectItem(root, "controls");
    assert(cJSON_IsTrue(cJSON_GetObjectItem(root, "mismatch")));
    assert(cJSON_IsFalse(cJSON_GetObjectItem(cJSON_GetArrayItem(controls, 0), "enabled")));
    cJSON_Delete(root);
    free(line);

    // A section continued on the next bank names its part on both, and a bank
    // holding two sections names both: Vibra's Color page read 1 AUTO,
    // 2 ELEGIR, 3 GOLPES, 4 GOLPES in the Qt pager (2026-10-28).
    model.layout.headings = 3;
    model.layout.heading[0] = (struct desk_heading){.page=1,.bank=0,.section=0,.part=1,.parts=1};
    strcpy(model.layout.heading[0].text, "AUTO");
    model.layout.heading[1] = (struct desk_heading){.page=1,.bank=0,.section=1,.part=1,.parts=2};
    strcpy(model.layout.heading[1].text, "GOLPES");
    model.layout.heading[2] = (struct desk_heading){.page=1,.bank=1,.section=1,.part=2,.parts=2};
    strcpy(model.layout.heading[2].text, "GOLPES");
    line = desk_view_snapshot(&model, &speed, &setup);
    assert(line);
    root = cJSON_Parse(line);
    assert(root);
    cJSON *pager = cJSON_GetObjectItem(root, "pager");
    assert(cJSON_GetArraySize(pager) == 2);
    assert(strcmp(cJSON_GetObjectItem(cJSON_GetArrayItem(pager, 0), "caption")->valuestring,
                  "AUTO + GOLPES 1/2") == 0);
    assert(strcmp(cJSON_GetObjectItem(cJSON_GetArrayItem(pager, 1), "caption")->valuestring,
                  "GOLPES 2/2") == 0);
    cJSON_Delete(root);
    free(line);
    return 0;
}
