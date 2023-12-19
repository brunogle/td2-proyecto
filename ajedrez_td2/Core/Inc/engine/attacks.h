#ifndef ATTACKS_H
#define ATTACKS_H

#include "moves.h"

extern game_state_t engine_game_state;

int is_attacked(game_state_t * state, color_t by_color, square_t sq);

#endif