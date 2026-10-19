#include <stdint.h>
#include <stddef.h>
#include "canvas_blend.h"

/* Coverage depends only on radius and the corner pixel, never its color.
   Keep the small UI radii hot; large general-purpose shapes use the oracle. */
static unsigned char corner_masks[65][64 * 64];
static unsigned char corner_ready[65];
/* Per corner row, the first column with any coverage and the first fully
   covered one: an opaque shape fills from the latter and blends only the
   pixels between them. */
static unsigned char corner_any[65][64], corner_full[65][64];

static unsigned corner_coverage(int r, int x, int y) {
    if (!corner_ready[r]) {
        for (int j = 0; j < r; j++) {
            corner_any[r][j] = corner_full[r][j] = (unsigned char)r;
            for (int i = r - 1; i >= 0; i--) {
                unsigned inside = 0;
                for (int sy = 0; sy < 4; sy++) for (int sx = 0; sx < 4; sx++) {
                    int dx = (i - r) * 8 + sx * 2 + 1;
                    int dy = (j - r) * 8 + sy * 2 + 1;
                    inside += dx * dx + dy * dy <= r * r * 64;
                }
                corner_masks[r][j * r + i] = (unsigned char)inside;
                if (inside)
                    corner_any[r][j] = (unsigned char)i;
                if (inside == 16 && corner_full[r][j] == i + 1)
                    corner_full[r][j] = (unsigned char)i;
            }
        }
        corner_ready[r] = 1;
    }
    return corner_masks[r][y * r + x];
}

uint32_t canvas_over(uint32_t dst, uint32_t src) {
    unsigned sa = src >> 24;
    if (!sa) return dst;
    if (sa == 255) return src;
    if (!(dst >> 24)) return src;
    // Keep the destination weight unrounded until the final channel division.
    unsigned ws = sa * 255, wd = (dst >> 24) * (255 - sa), sum = ws + wd;
    unsigned a = (sum + 127) / 255;
    unsigned r = (((src >> 16) & 255) * ws + ((dst >> 16) & 255) * wd + sum / 2) / sum;
    unsigned g = (((src >> 8) & 255) * ws + ((dst >> 8) & 255) * wd + sum / 2) / sum;
    unsigned b = ((src & 255) * ws + (dst & 255) * wd + sum / 2) / sum;
    return (a << 24) | (r << 16) | (g << 8) | b;
}

void canvas_blend(struct canvas *c, int x, int y, uint32_t col) {
    if (!canvas_visible(c, x, y)) return;
    uint32_t *p = &c->px[(size_t)y * c->w + x];
    *p = canvas_over(*p, col);
}

void canvas_blend_rect(struct canvas *c, int x, int y, int w, int h, uint32_t col) {
    if (w <= 0 || h <= 0) return;
    int64_t right = (int64_t)x + w, bottom = (int64_t)y + h;
    if (right > c->w) right = c->w;
    if (bottom > c->h) bottom = c->h;
    for (int j = y < 0 ? 0 : y; j < bottom; j++)
        for (int i = x < 0 ? 0 : x; i < right; i++) canvas_blend(c, i, j, col);
}

void canvas_blend_round_rect(struct canvas *c, int x, int y, int w, int h, int r, uint32_t col) {
    if (w <= 0 || h <= 0) return;
    if (r < 0) r = 0;
    if (r > w / 2) r = w / 2;
    if (r > h / 2) r = h / 2;
    int64_t right = (int64_t)x + w, bottom = (int64_t)y + h;
    if (right > c->w) right = c->w;
    if (bottom > c->h) bottom = c->h;
    /* Every tile, panel and pill on the desk is opaque: its interior is a
       plain fill, and of each corner row only the pixels the arc cuts blend.
       A translucent or very large shape keeps the general path below. */
    int opaque = (col >> 24) == 255;
    if (opaque && r <= 64) {
        if (r > 0)
            corner_coverage(r, 0, 0);
        for (int j = y < 0 ? 0 : y; j < bottom; j++) {
            int64_t ry = (int64_t)j - y;
            if (ry >= r && ry < (int64_t)h - r) {
                canvas_fill_rect(c, x, j, w, 1, col);
                continue;
            }
            int cy = (int)(ry < r ? ry : h - 1 - ry);
            int any = corner_any[r][cy], full = corner_full[r][cy];
            canvas_fill_rect(c, x + full, j, w - 2 * full, 1, col);
            for (int i = any; i < full; i++) {
                uint32_t src = (((col >> 24) * corner_masks[r][cy * r + i] / 16) << 24) | (col & 0x00FFFFFFu);
                canvas_blend(c, x + i, j, src);
                canvas_blend(c, x + w - 1 - i, j, src);
            }
        }
        return;
    }
    uint32_t last_src = 0, last_dst = 0, last_result = 0;
    int cached = 0;
    for (int j = y < 0 ? 0 : y; j < bottom; j++) {
        int band = (int64_t)j >= (int64_t)y + r && (int64_t)j < (int64_t)y + h - r;
        int64_t run0 = band ? x : (int64_t)x + r, run1 = band ? (int64_t)x + w : (int64_t)x + w - r;
        if (opaque && run1 > run0)
            canvas_fill_rect(c, (int)run0, j, (int)(run1 - run0), 1, col);
        for (int i = x < 0 ? 0 : x; i < right; i++) {
            if (opaque && i >= run0 && i < run1) {
                i = (int)run1 - 1;
                continue;
            }
            int inside = 0;
            /* Only the four corner squares need subpixel coverage. */
            if (((int64_t)i >= (int64_t)x + r && (int64_t)i < (int64_t)x + w - r) ||
                ((int64_t)j >= (int64_t)y + r && (int64_t)j < (int64_t)y + h - r)) {
                inside = 16;
            } else if (r <= 64) {
                int64_t cx = (int64_t)i - x, cy = (int64_t)j - y;
                if (cx >= r) cx = w - 1 - cx;
                if (cy >= r) cy = h - 1 - cy;
                inside = (int)corner_coverage(r, (int)cx, (int)cy);
            } else for (int sy = 0; sy < 4; sy++) for (int sx = 0; sx < 4; sx++) {
                double px = (double)i - x + (sx + 0.5) / 4;
                double py = (double)j - y + (sy + 0.5) / 4;
                double cx = px < r ? r : px > w - r ? w - r : px;
                double cy = py < r ? r : py > h - r ? h - r : py;
                double dx = px - cx, dy = py - cy;
                if (dx * dx + dy * dy <= (double)r * r) inside++;
            }
            if (!inside) continue;
            unsigned a = (col >> 24) * inside / 16;
            uint32_t src = (a << 24) | (col & 0x00FFFFFFu);
            if (!canvas_visible(c, i, j)) continue;
            uint32_t *dst = &c->px[(size_t)j * c->w + i];
            /* Solid glass/tile runs have identical operands: avoid repeating
               three software integer divisions per pixel on Cortex-A7. */
            if (!cached || last_src != src || last_dst != *dst) {
                last_src = src; last_dst = *dst;
                last_result = canvas_over(*dst, src); cached = 1;
            }
            *dst = last_result;
        }
    }
}
