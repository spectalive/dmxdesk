// What the desk asks of a lighting console, in the desk's words rather than
// any console's wire format: a console adapter turns each into its own
// protocol. ids are the console's own widget or function ids, as the
// snapshot names them.
#ifndef DESK_COMMAND_H
#define DESK_COMMAND_H

enum desk_command_kind {
    DESK_CMD_TOGGLE,        // id: a toggle button, pressed once
    DESK_CMD_STOP_ALL,      // id: the console's stop-all button
    DESK_CMD_FLASH,         // id: a flash button; value: 1 down, 0 up
    DESK_CMD_FUNCTION,      // id: a function; value: 1 start, 0 stop
    DESK_CMD_GRAND_MASTER,  // value: 0-255
    DESK_CMD_SPEED_MS,      // id: a speed dial; value: its time in ms
    DESK_CMD_SPEED_FACTOR,  // id: a speed dial; value: its multiplier, 0..10
};

struct desk_command {
    enum desk_command_kind kind;
    int id;
    int value;
};

#endif
