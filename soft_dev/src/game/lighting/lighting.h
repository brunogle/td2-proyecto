#ifndef LIGHTING_H
#define LIGHTING_H

#include <stdint.h>
#include "../engine/engine.h"

#define LIGHTING_IDLE_STATE 0
#define LIGHTING_LIFTED_STATE 1
#define LIGHTING_ERROR_STATE 2
#define LIGHTING_CAPTURE_STATE 3
#define LIGHTING_CPU_THINKING_STATE 4
#define LIGHTING_CPU_LIFT_FROM_STATE 5
#define LIGHTING_CPU_LIFT_CAPTURED_STATE 6
#define LIGHTING_CPU_PLACE_TO_STATE 7
#define LIGHTING_GAME_FINISHED_STATE 8


typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} led_color;

#define WHITE_COLOR {200, 200, 200}
#define BLACK_COLOR {50, 50, 50}
#define VALID_COLOR {50, 150, 50}
#define LIFTED_COLOR {150, 150, 50}
#define MISSING_COLOR {200, 100, 50}
#define INVALID_COLOR {200, 50, 50}


#define WHITE_ID 0
#define BLACK_ID 1
#define VALID_ID 2
#define LIFTED_ID 3
#define MISSING_ID 4
#define INVALID_ID 5

extern led_color color_from_id[6];

extern int square_colors[8][8];

void lighting_refresh();

void lighting_set_state(char state);
void lighting_piece_lifted_square(uint8_t square);
void lighting_set_cpu_movement(uint8_t from, uint8_t to);
void lighting_set_valid_moves(move_t * moves, int total_valid_moves);
void lighting_set_winner(char winner);
void lighting_set_output(led_color led_output_array[8][8]);

#endif
