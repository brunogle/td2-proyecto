#include "./engine/engine.h"

#include "./engine/attacks.h"
#include "./engine/board.h"
#include "./engine/interface.h"
#include "./engine/moves.h"
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

game_state_t engine_game_state;

const int8_t engine_piece_scores[] = {0, QUEEN_SCORE, ROOK_SCORE, BISHOP_SCORE, KNIGHT_SCORE, PAWN_SCORE, 0};

void engine_reset(){
    load_fen(&engine_game_state, STARTFEN);
}

char engine_move_piece(move_t move){
    return make_move(&engine_game_state, move, 1);
}

move_t pseudo_legal_moves[255] = {0};

int engine_list_moves(move_t moves[], char only_legal){
	only_legal = 1;
    if(only_legal){

        game_state_t test_game_state = engine_game_state;
        //game_state_t test_game_state;
        //memcpy(&test_game_state, &engine_game_state, sizeof(game_state_t));

        int num_pseudo_legal = generate_moves(&engine_game_state, pseudo_legal_moves);




        int num_legal = 0;
        for(int i = 0; i < num_pseudo_legal; i++){

        	memcpy(&test_game_state, &engine_game_state, sizeof(game_state_t));
            make_move(&test_game_state, pseudo_legal_moves[i], 0);
            uint8_t king_pos = 0;
            while(!(test_game_state.pieces[king_pos] == KING && test_game_state.color[king_pos] == engine_game_state.side_to_move)){
                king_pos++;
            }
            if(!is_attacked(&test_game_state, test_game_state.side_to_move, king_pos)){
                moves[num_legal] = pseudo_legal_moves[i];
                num_legal++;
            }

        }
        return num_legal;
    }
    else{
        return generate_moves(&engine_game_state, moves);
    }
}

char engine_get_piece(uint8_t square){
    return engine_game_state.pieces[square];
}

int engine_eval(game_state_t * game_state){

    //
    int score = 0;

    for (uint8_t row=0; row < 8; row++){
        for (uint8_t col=0; col < 8; col++) {

            uint8_t sq = COORD2SQ(row, col);

            if(game_state->color[sq] != COLOR_EMPTY){
                int piece_score = engine_piece_scores[game_state->pieces[sq]];
                if(game_state->color[sq] == WHITE)
                    score += piece_score;
                else
                    score -= piece_score;
            }
        }
    }



    if(game_state->side_to_move == WHITE)
        return score;
    else
        return -score;
}


int engine_pv_search(int alpha, int beta, int depth){

    if(depth == 0){
        return 0;
    }

    return 0;

}

#define SEARCH_DEPTH 3
/*
move_t moves_calc[SEARCH_DEPTH][100];

game_state_t test_game_state[SEARCH_DEPTH];

int engine_negamax_seach(game_state_t * game_state, int depth, int alpha, int beta){
    if(depth == 0)
        return engine_eval(game_state);

    int num_moves = engine_list_moves(moves_calc[depth], 1);

    if(num_moves == 0){
        return MIN_EVAL;
    }

    //Idealmente ordenar moviminetos
    int score = MIN_EVAL;
    for(int i = 0; i < num_moves; i++){
        move_t test_move = moves_calc[depth][i];
        test_game_state[depth] = *game_state;
        make_move(&test_game_state[depth], test_move, 0);
        score = -engine_negamax_seach(&test_game_state[depth], depth - 1, -beta, -alpha);
        if(score >= beta)
        	return beta;
        if(score > alpha)
            alpha = score;
    }
    return alpha;
}

*/

move_t search_valid_moves[255];
int engine_negamax_seach2(game_state_t game_state, int depth, int alpha, int beta){

	move_t moves_calc[100];

	game_state_t test_game_state;

    if(depth == 0)
        return engine_eval(&game_state);

    int num_moves = engine_list_moves(moves_calc, 1);

    if(num_moves == 0){
        return MIN_EVAL;
    }

    //Idealmente ordenar moviminetos
    int score = MIN_EVAL;
    for(int i = 0; i < num_moves; i++){
        move_t test_move = moves_calc[i];
        test_game_state = game_state;
        make_move(&test_game_state, test_move, 0);
        score = -engine_negamax_seach2(test_game_state, depth - 1, -beta, -alpha);
        if(score >= beta)
        	return beta;
        if(score > alpha)
            alpha = score;
    }
    return alpha;
}



move_t engine_search(){
    //Placeholder for actual search
    int num_moves = generate_moves(&engine_game_state, search_valid_moves);

    move_t best_move;
    int best_score = MIN_EVAL;

    game_state_t test_game_state;

    for(int i = 0; i < num_moves; i++){
        move_t test_move = search_valid_moves[i];
        test_game_state = engine_game_state;
        make_move(&test_game_state, test_move, 0);
        int score = -engine_negamax_seach2(test_game_state, SEARCH_DEPTH - 1, MIN_EVAL, MAX_EVAL);
        if(score > best_score){
            best_score = score;
            best_move = search_valid_moves[i];
        }
    }

    return best_move;

}
