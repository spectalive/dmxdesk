#!/bin/sh
# Build the QLC+ to OSC surface converter (tools/qlc-osc-surface.c) for this
# machine, into output/qlc-osc-surface. It runs on the bench, not the tablet,
# and needs only a C compiler and the cJSON that tools/get-cjson.sh fetches.
set -eu
here=$(cd "$(dirname "$0")/.." && pwd)
CJSON_DIR=${CJSON_DIR:-$here/tools/vendor/cjson}
[ -f "$CJSON_DIR/cJSON.c" ] || { echo "run tools/get-cjson.sh first" >&2; exit 1; }
out=$here/output/qlc-osc-surface-build
mkdir -p "$out/include/cjson"
ln -sf "$CJSON_DIR/cJSON.h" "$out/include/cjson/cJSON.h"
rm -f "$here/output/qlc-osc-surface"
# cJSON is compiled on its own terms, as in the host tests: -Werror is for our code.
${CC:-cc} -std=gnu99 -O2 -w -c "$CJSON_DIR/cJSON.c" -o "$out/cJSON.o"
cd "$here/core"
${CC:-cc} -std=gnu99 -Wall -Wextra -Werror -O2 -I. -I"$out/include" \
    -o "$here/output/qlc-osc-surface" "$here/tools/qlc-osc-surface.c" \
    vcjson.c qlc_surface_of.c qlc_surface_kind.c desk_surface_free.c desk_surface_find.c \
    qlc_osc_surface_json.c qlc_osc_item_json.c qlc_osc_channel.c qlc_osc_factor_input.c \
    qlc_osc_channels_write.c osc_kind_word.c \
    osc_map_parse.c osc_map_parse_items.c osc_map_parse_functions.c osc_item_parse.c \
    osc_function_parse.c osc_factor_presses_parse.c osc_factor_key.c osc_kind_of.c \
    osc_json_int.c osc_json_bool.c osc_json_address.c osc_json_range.c osc_map_free.c \
    "$out/cJSON.o" -lm
echo "built output/qlc-osc-surface"
