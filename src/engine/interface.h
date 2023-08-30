#ifndef INTERFACE_H
#define INTERFACE_H

#include "board.h"



int load_fen(game_state_t * state, const char * fen);
void display_board(game_state_t state);

uint8_t str_to_square(char * str);
move_t str_to_move(char * str);

int move_to_str(move_t move, char * str);
int square_to_string(uint8_t sq, char * str);



#endif
