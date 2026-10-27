#define _GNU_SOURCE
#include "desk_dump_ppm.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

void desk_dump_ppm(const struct canvas *c, const char *path) {
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC | O_NOFOLLOW | O_CLOEXEC, 0644);
    if (fd < 0) {
        perror("DMXDESK_DUMP");
        return;
    }
    FILE *f = fdopen(fd, "wb");
    if (!f) {
        close(fd);
        return;
    }
    fprintf(f, "P6\n%d %d\n255\n", c->w, c->h);
    for (int i = 0; i < c->w * c->h; i++) {
        uint32_t p = c->px[i];
        unsigned char rgb[3] = { (p >> 16) & 0xff, (p >> 8) & 0xff, p & 0xff };
        fwrite(rgb, 1, 3, f);
    }
    fclose(f);
}
