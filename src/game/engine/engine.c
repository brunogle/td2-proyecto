#include "engine.h"

#include "interface.h"
#include <stdint.h>

game_state_t game_state;

void reset_game(){
    load_fen(&game_state, STARTFEN);
}

char move_piece(move_t move){
    return make_move(&game_state, move, 1);
}

int list_moves(move_t * moves){
    return generate_moves(&game_state, moves);
}


char get_piece(uint8_t square){
    return game_state.pieces[square];
}
