#!/bin/sh
# Host tests for the DMX desk. Run from anywhere; paths resolve from the repo.
set -eu
here=$(cd "$(dirname "$0")/.." && pwd)
out=${TEST_OUT:-$here/output/dmx-desk-host}
mkdir -p "$here/output" "$out/include/cjson" "$out/include/stb"
STB_DIR=${STB_DIR:-$here/tools/vendor/stb}
[ -f "$STB_DIR/stb_truetype.h" ] || { echo "stb_truetype.h not in $STB_DIR; run tools/get-stb.sh first" >&2; exit 1; }
ln -sf "$STB_DIR/stb_truetype.h" "$out/include/stb/stb_truetype.h"
CC=${CC:-cc}
out=$(cd "$out" && pwd)
export TEST_OUT="$out"
# The Wi-Fi tests bind Unix sockets under TMPDIR, and sun_path holds 104 bytes
# on macOS: a checkout at a deep path would truncate them, so keep it short.
TMPDIR=$(mktemp -d /tmp/dmxdesk-test.XXXXXX)
export TMPDIR
trap 'rm -rf "$TMPDIR"' EXIT
# cJSON comes from Buildroot on the device; the host compiles the same pinned
# sources, fetched by tools/get-cjson.sh.
CJSON_DIR=${CJSON_DIR:-$here/tools/vendor/cjson}
[ -f "$CJSON_DIR/cJSON.c" ] || { echo "run tools/get-cjson.sh first" >&2; exit 1; }
ln -sf "$CJSON_DIR/cJSON.h" "$out/include/cjson/cJSON.h"
# Third-party source is compiled on its own terms: our -Werror is about our
# code, and cJSON 1.7.19 still calls sprintf.
"$CC" -std=gnu99 -O1 -w -fsanitize=address,undefined -c "$CJSON_DIR/cJSON.c" \
	-o "$out/cJSON.o"
cd "$here"
fail=0
for t in "$here"/tests/dmx-desk/*_test.c; do
    name=$(basename "$t" .c)
    srcs=$(sed -n 's|^// SOURCES: ||p' "$t")
    set --
    # Full-panel property tests make millions of calls through the real models.
    # Keep sanitizers and warnings, but let those tests request optimization.
    if [ "$(sed -n 's|^// OPTIMIZE: ||p' "$t")" = 2 ]; then
        set -- -O2
    fi
    for s in $srcs; do set -- "$@" "$here/src/$s"; done
    if "$CC" -std=gnu99 -Wall -Wextra -Werror -fsanitize=address,undefined \
        -I "$here/src" -I "$here/tests/dmx-desk" -I "$out/include" \
        -o "$out/$name" "$t" "$@" "$out/cJSON.o" -lm && "$out/$name"; then
        echo "PASS $name"
    else
        echo "FAIL $name"
        fail=1
    fi
done
exit "$fail"
