#include "engine.h"

#include "attacks.h"
#include "board.h"
#include "interface.h"
#include "moves.h"
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

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

int engine_list_moves(move_t * moves, char only_legal){
    if(only_legal){
        move_t pseudo_legal_moves[128];
        game_state_t test_game_state = engine_game_state;

        int num_pseudo_legal = generate_moves(&engine_game_state, pseudo_legal_moves);

        


        int num_legal = 0;
        for(int i = 0; i < num_pseudo_legal; i++){
            test_game_state = engine_game_state;
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

}

#define SEARCH_DEPTH 3
#define MAX_POSSIBLE_MOVEMENTS 100


int x = 0;


int searching_depth;

move_t pv_table[SEARCH_DEPTH][SEARCH_DEPTH];


void update_pv_table(move_t new_best_move, int ply, char is_leaf_node){

    pv_table[ply][ply] = new_best_move;
    if(is_leaf_node){//If node analyzed is a terminal node
        pv_table[ply][ply + 1].from = -1; //If move bitarray is set to -1, this is a terminal node used ony for killer moves.     
    }
    else{
        for(int j = ply + 1; j < searching_depth; j++){
            pv_table[ply][j] = pv_table[ply+1][j];
        }
    }
}


int negamax(game_state_t prev_node_state, int alpha, int beta, int depth) {
    x++;

    //Leaf node conditions

    if(depth == 0){
        return engine_eval(&prev_node_state);
    }


    int ply = searching_depth - depth;

    int win_score = MIN_EVAL + ply; //Further into the tree, the lower the win_score is

    //If window is an infinite window, limit the values to win score
    if (alpha < win_score) alpha = win_score;
    if (beta > -win_score - 1) beta = -win_score - 1;
    if (alpha >= beta){
        return alpha;
    } 

    //Generate moves
    move_t possible_moves[MAX_POSSIBLE_MOVEMENTS];
    int num_moves = engine_list_moves(possible_moves, 1);

    if(num_moves == 0){
        return MIN_EVAL;
    }



    for(int i = 0; i < num_moves; i++){
        game_state_t node_state = prev_node_state;
        make_move(&node_state, possible_moves[i], 1);

        int score = -negamax(node_state, -beta, -alpha, depth - 1);

        //Update alpha (better child node has been found)
        if(score > alpha){
            alpha = score;
            
            update_pv_table(possible_moves[i], ply, score == MAX_EVAL);

            if(score >= beta){
                //If condition is met, this node is garanteed to have a score smaller than
                //another previously searched sibling node
                
                return beta;
            }
        }
    }
    return alpha;

}

int root_search(game_state_t prev_node_state, int depth) {

    //Leaf node conditions

    int ply = 0;
    int alpha = MIN_EVAL;
    int beta = MAX_EVAL;

    int win_score = MAX_EVAL - ply; //Further into the tree, the lower the win_score is


    //Generate moves
    move_t possible_moves[MAX_POSSIBLE_MOVEMENTS];
    int num_moves = engine_list_moves(possible_moves, 1);

    for(int i = 0; i < num_moves; i++){
        game_state_t node_state = prev_node_state;
        make_move(&node_state, possible_moves[i], 1);

        int score = -negamax(node_state, -beta, -alpha, depth - 1);

        //Beta cut-off


        //Update alpha (better child node has been found)
        if(score > alpha){
            alpha = score;

            update_pv_table(possible_moves[i], ply, score == MAX_EVAL);

            if(score >= beta){
                //If condition is met, this node is garanteed to have a score smaller than
                //another previously searched sibling node

                return beta;
            }
        }
    }
    return alpha;

}

move_t engine_search(){

    game_state_t state = engine_game_state;

    x = 0;
    move_t possible_moves[MAX_POSSIBLE_MOVEMENTS];
    int num_moves = engine_list_moves(possible_moves, 1);

    int scores[MAX_POSSIBLE_MOVEMENTS];

    int score;

    for(int i = 0; i < SEARCH_DEPTH; i++){
        for(int j = 0; j < SEARCH_DEPTH; j++){
            pv_table[i][j].to = -1;
        }            
    }

    for(int depth = 2; depth <= SEARCH_DEPTH; depth++){
        //std::cout << "searching depth " << depth << std::endl;
        searching_depth = depth;
        score =  root_search(state, depth);
        if(abs(score) > MAX_EVAL - 100){
            break;
        }
    }
    

    move_t best_move = pv_table[0][0];

    //std::cout << "pv: ";
    return best_move;

}