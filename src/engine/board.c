#include "board.h"

#include <stdio.h>


int clear_game_state(game_state_t * state){
    for(int i = 0; i < 128; i++){
        state->color[i] = COLOR_EMPTY;
        state->pieces[i] = PIECE_EMPTY;
    }
    return 1;
}

int place_piece(game_state_t * state, piece_t piece, color_t color, uint8_t square){
    state->pieces[square] = piece;
    state->color[square] = color;
    return 1;
}

int remove_piece(game_state_t * state, uint8_t square){
    state->pieces[square] = PIECE_EMPTY;
    state->color[square] = COLOR_EMPTY;
    return 1;
}
