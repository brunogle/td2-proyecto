#ifndef LIGHTING_H
#define LIGHTING_H

#include <inttypes.h>
#include "../engine/engine.h"

#define LIGHTING_IDLE_STATE 0
#define LIGHTING_LIFTED_STATE 1
#define LIGHTING_ERROR_STATE 2


#define WHITE_COLOR {200, 200, 200, 255}
#define BLACK_COLOR {50, 50, 50, 255}
#define VALID_COLOR {50, 150, 50, 255}
#define LIFTED_COLOR {150, 150, 50, 255}
#define MISSING_COLOR {200, 100, 50, 255}
#define INVALID_COLOR {200, 50, 50, 255}


#define WHITE_ID 0
#define BLACK_ID 1
#define VALID_ID 2
#define LIFTED_ID 3
#define MISSING_ID 4
#define INVALID_ID 5


extern float color_from_id[6][4];

extern int square_colors[8][8];

void refresh_board_coloring();

void set_lighting_state(char state);
void set_square_lifted(uint8_t square);
void set_valid_moves(move_t * moves, int total_valid_moves);

#endif
