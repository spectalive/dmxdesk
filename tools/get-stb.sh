#!/bin/sh
# Fetch stb_truetype.h into tools/vendor/stb/ for the host tests.
#
# The commit is the one Buildroot 2026.02.3's stb package pins, so the host
# rasterises text with the same header the target build compiles.
set -eu

VENDOR=$(dirname "$0")/vendor/stb
STB_COMMIT=8b5f1f37b5b75829fc72d38e7b5d4bcbf8a26d55
BASE=https://raw.githubusercontent.com/nothings/stb/$STB_COMMIT

mkdir -p "$VENDOR"
curl -sfL "$BASE/stb_truetype.h" -o "$VENDOR/stb_truetype.h"
