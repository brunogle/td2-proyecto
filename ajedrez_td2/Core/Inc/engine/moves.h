#ifndef MOVES_H
#define MOVES_H

#define DIR_N 16
#define DIR_S -16
#define DIR_E 1
#define DIR_W -1

#define DIR_NN 32
#define DIR_SS -32

#define DIR_NE 17
#define DIR_SE -15
#define DIR_SW -17
#define DIR_NW 15

#define DIR_NNW 31
#define DIR_NNE 33
#define DIR_NEE 18
#define DIR_SEE -14
#define DIR_SSE -31 
#define DIR_SSW -33
#define DIR_SWW -18
#define DIR_NWW 14

#include "board.h"

extern int8_t move_directions[5][8];

int make_move(game_state_t * state, move_t move, uint8_t safe);

int generate_moves(game_state_t * state, move_t * moves);

piece_t check_promotion();

#endif
