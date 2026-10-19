// SOURCES: touch_flip.c
// A flipped axis mirrors inside the panel: on 1024x600 column 0 becomes 1023
// and 1023 becomes 0, row 0 becomes 599 and 599 becomes 0. Subtracting from
// the width instead put raw 0 at x 1024, one past the last column, and left
// column 0 unreachable (fixed 2026-10-19; latent in the desk, which never
// sets flipped, but glcube's orientation path reaches the same code).
#include <assert.h>
#include <stdio.h>

#include "touch_flip.h"

int main(void) {
    struct touch_flip flip;

    // Screen pixels already (the vendor driver): scale 1.0.
    assert(touch_flip_configure(&flip, 1024, 600, 1023, 599, "xy") == 0);
    assert(touch_flip_value(&flip, TOUCH_AXIS_X, 0, 1) == 1023);
    assert(touch_flip_value(&flip, TOUCH_AXIS_X, 1023, 1) == 0);
    assert(touch_flip_value(&flip, TOUCH_AXIS_Y, 0, 1) == 599);
    assert(touch_flip_value(&flip, TOUCH_AXIS_Y, 599, 1) == 0);
    assert(touch_flip_value(&flip, TOUCH_AXIS_X, 0, 0) == 0);
    assert(touch_flip_value(&flip, TOUCH_AXIS_Y, 599, 0) == 599);

    // The controller's own grid (mainline silead, 1663x895): the scaled
    // extremes still land on the first and last pixel, flipped or not.
    assert(touch_flip_configure(&flip, 1024, 600, 1663, 895, "xy") == 0);
    assert(touch_flip_value(&flip, TOUCH_AXIS_X, 0, 1) == 1023);
    assert(touch_flip_value(&flip, TOUCH_AXIS_X, 1663, 1) == 0);
    assert(touch_flip_value(&flip, TOUCH_AXIS_Y, 0, 1) == 599);
    assert(touch_flip_value(&flip, TOUCH_AXIS_Y, 895, 1) == 0);
    assert(touch_flip_value(&flip, TOUCH_AXIS_X, 1663, 0) == 1023);
    assert(touch_flip_value(&flip, TOUCH_AXIS_Y, 895, 0) == 599);

    // One axis only: the other passes through.
    assert(touch_flip_configure(&flip, 1024, 600, 1023, 599, "x") == 0);
    assert(touch_flip_value(&flip, TOUCH_AXIS_X, 0, 1) == 1023);
    assert(touch_flip_value(&flip, TOUCH_AXIS_Y, 0, 1) == 0);
    assert(touch_flip_configure(&flip, 1024, 600, 1023, 599, "y") == 0);
    assert(touch_flip_value(&flip, TOUCH_AXIS_X, 0, 1) == 0);
    assert(touch_flip_value(&flip, TOUCH_AXIS_Y, 0, 1) == 599);
    assert(touch_flip_configure(&flip, 1024, 600, 1023, 599, "none") == 0);
    assert(touch_flip_value(&flip, TOUCH_AXIS_X, 0, 1) == 0);
    assert(touch_flip_value(&flip, TOUCH_AXIS_Y, 599, 1) == 599);

    printf("touch_flip ok\n");
    return 0;
}
