#include "qlc_encode_command.h"

#include "qlc_codec.h"

int qlc_encode_command(char *buf, size_t cap, const struct desk_command *cmd) {
    switch (cmd->kind) {
    case DESK_CMD_TOGGLE:       return qlc_encode_toggle(buf, cap, cmd->id);
    case DESK_CMD_STOP_ALL:     return qlc_encode_stop_all(buf, cap, cmd->id);
    case DESK_CMD_FLASH:        return qlc_encode_flash(buf, cap, cmd->id, cmd->value);
    case DESK_CMD_FUNCTION:     return qlc_encode_function_status(buf, cap, cmd->id, cmd->value);
    case DESK_CMD_GRAND_MASTER: return qlc_encode_grand_master(buf, cap, cmd->value);
    case DESK_CMD_SPEED_MS:     return qlc_encode_speed_ms(buf, cap, cmd->id, cmd->value);
    case DESK_CMD_SPEED_FACTOR: return qlc_encode_speed_factor(buf, cap, cmd->id, cmd->value);
    }
    return -1;
}
