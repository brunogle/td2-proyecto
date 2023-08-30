#include "engine.h"

#include "interface.h"
#include <stdint.h>

game_state_t game_state;

void engine_reset(){
    load_fen(&game_state, STARTFEN);
}

char engine_move_piece(move_t move){
    return make_move(&game_state, move, 1);
}

int engine_list_moves(move_t * moves){
    return generate_moves(&game_state, moves);
}

char engine_get_piece(uint8_t square){
    return game_state.pieces[square];
}
