// SOURCES: showmap.c showmap_release_to_parse.c showmap_release_to_prune.c showmap_validate.c showmap_check_priority.c desk_model.c desk_master_level_at.c vcjson.c desk_damage_running.c desk_panel_holds.c
// 2026-10-21: a Fiesta colour step must not reclaim the 7R wheel during a
// white Flash, and a disconnected tablet must not leave that Flash running.
#include <assert.h>
#include <string.h>

#include "desk_model.h"
#include "showmap.h"
#include "showmap_validate.h"
#include "vcjson.h"

int main(void) {
    const char *map_json =
        "{\"schema\":2,\"qlcVersion\":\"5.2.2\",\"show\":{\"key\":\"t\",\"workspace\":\"t.qxw\",\"sha256\":\"\"},"
        "\"grandMaster\":null,\"stopAll\":null,\"pages\":[{\"key\":\"live\",\"title\":\"LIVE\",\"sections\":[{\"key\":\"accents\",\"title\":\"HITS\",\"solo\":null,\"controls\":[\"flash\"]}]}],"
        "\"controls\":{\"flash\":{\"widget\":300,\"function\":813,\"functionType\":\"Chaser\",\"action\":\"toggle\","
        "\"caption\":\"FLASH\",\"detail\":\"\",\"role\":\"burst\",\"burstMs\":8000,\"source\":\"flash\","
        "\"priorityWidget\":47,\"priorityFunction\":2,\"solo\":null,\"key\":null,\"enabled\":true,\"reason\":\"\",\"swatches\":[]}}}";
    const char *old_vc =
        "{\"app\":{\"version\":\"5.2.2\"},\"pages\":[{\"id\":0,\"children\":["
        "{\"id\":300,\"typeId\":1,\"caption\":\"Desk flash\",\"functionId\":813,\"actionType\":0,\"visible\":false},"
        "{\"id\":47,\"typeId\":1,\"caption\":\"FLASH\",\"functionId\":2,\"actionType\":1}]}]}";
    const char *bounded_vc =
        "{\"app\":{\"version\":\"5.2.2\"},\"pages\":[{\"id\":0,\"children\":["
        "{\"id\":300,\"typeId\":1,\"caption\":\"Desk flash\",\"functionId\":813,\"actionType\":0,\"visible\":false},"
        "{\"id\":47,\"typeId\":1,\"caption\":\"FLASH\",\"functionId\":2,\"actionType\":1,\"flashOverride\":true,\"flashForceLTP\":true,\"flashMaxHoldMs\":8000}]}]}";
    const char *unprioritized_vc =
        "{\"app\":{\"version\":\"5.2.2\"},\"pages\":[{\"id\":0,\"children\":["
        "{\"id\":300,\"typeId\":1,\"functionId\":813,\"actionType\":0},"
        "{\"id\":47,\"typeId\":1,\"functionId\":2,\"actionType\":1,\"flashMaxHoldMs\":8000}]}]}";
    struct show_map map;
    struct vc_doc console;
    struct desk_model model;
    assert(showmap_parse(map_json, strlen(map_json), &map) == 0);
    assert(vc_parse(old_vc, strlen(old_vc), &console) == 0);
    showmap_build(&model, &map, &console);
    assert(!model.control[0].enabled);
    assert(strcmp(model.control[0].reason, "master timeout missing") == 0);
    vc_free(&console);
    assert(vc_parse(unprioritized_vc, strlen(unprioritized_vc), &console) == 0);
    showmap_build(&model, &map, &console);
    assert(!model.control[0].enabled);
    assert(strcmp(model.control[0].reason, "master priority missing") == 0);
    vc_free(&console);
    assert(vc_parse(bounded_vc, strlen(bounded_vc), &console) == 0);
    showmap_build(&model, &map, &console);
    assert(model.control[0].enabled);
    assert(model.control[0].priority_widget_id == 47);
    vc_free(&console);
    return 0;
}
