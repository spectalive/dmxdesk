#include "linux_desk_stopping.h"

#include "linux_desk.h"

int linux_desk_stopping(void *ctx) {
    return *((struct linux_desk *)ctx)->stop != 0;
}
