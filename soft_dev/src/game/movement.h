#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdint.h>

#define WAIT_STATE 0
#define PIECE_LIFTED_STATE 1
#define PIECE_CAPTURED_STATE 2
#define ERROR_STATE 3
#define CAPUTRE_LIEFTED_STATE 4
#define CPU_THINKING_STATE 5
#define CPU_LIFT_FROM_STATE 6
#define CPU_LIFT_CAPTURED_STATE 7
#define CPU_PLACE_TO_STATE 8
#define GAME_FINISHED_STATE 9

typedef enum enum_piece_change_type{
    NONE = 0,
    REMOVED = 1,
    PLACED = 2
} piece_change_type;

typedef struct piece_change_t{
    uint8_t square_affected;
    piece_change_type piece_action;
} piece_change_t;

extern void (*get_sensors)(char[8]);

piece_change_t get_last_sensor_change();

void get_expected_sensor_state();

void movement_fsm();

#endif
