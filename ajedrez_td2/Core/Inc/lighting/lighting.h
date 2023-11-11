#ifndef LIGHTING_H
#define LIGHTING_H

#include <stdint.h>
#include "../engine/engine.h"

#define LIGHTING_IDLE_STATE 0
#define LIGHTING_LIFTED_STATE 1
#define LIGHTING_ERROR_STATE 2
#define LIGHTING_CAPTURE_STATE 3

typedef uint32_t led_color;

#define WHITE_COLOR 0xFFFFFF
#define BLACK_COLOR 0x000000
#define VALID_COLOR 0x00FF00
#define LIFTED_COLOR 0xFFFF00
#define MISSING_COLOR 0xFF8000
#define INVALID_COLOR 0xFF0000


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
void set_valid_moves(move_t * moves, int total_valid_moves);

void lighting_set_output(led_color led_output_array[8][8]);

#endif
