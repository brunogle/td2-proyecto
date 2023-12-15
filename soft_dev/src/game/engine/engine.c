#include "engine.h"

#include "board.h"
#include "interface.h"
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

game_state_t game_state;

const int8_t engine_piece_scores[] = {0, QUEEN_SCORE, ROOK_SCORE, BISHOP_SCORE, KNIGHT_SCORE, PAWN_SCORE, 0};

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

int engine_eval(){

    //
    int score = 0;

    for (uint8_t row=0; row < 8; row++){
        for (uint8_t col=0; col < 8; col++) {

            uint8_t sq = COORD2SQ(row, col);

            if(game_state.color[sq] != PIECE_EMPTY){
                int piece_score = engine_piece_scores[game_state.pieces[sq]];
                if(game_state.color[sq] == WHITE)
                    score += piece_score;
                else
                    score -= piece_score;
            }
        }
    }

    

    if(game_state.side_to_move == WHITE)
        return score;
    else
        return -score;
}

int engine_pv_search(int alpha, int beta, int depth){
    
    if(depth == 0){
        return 0;
    }

}

move_t search_valid_moves[255];


move_t engine_search(){
    //Placeholder for actual search
    int num_moves = generate_moves(&game_state, search_valid_moves);

    return search_valid_moves[rand()%num_moves];

}