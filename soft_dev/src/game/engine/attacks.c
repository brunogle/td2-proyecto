#include "attacks.h"
#include "board.h"
#include "moves.h"
#include <stdint.h>


int is_attacked(game_state_t * state, color_t by_color, square_t sq){
    square_t from_square;


    if(by_color == WHITE){
        if(SQ2ROW(sq) > 0){
            if(SQ2COL(sq) > 0){
                if( state->pieces[sq - 17] == PAWN &&
                    state->color[sq - 17] == by_color){
                    return 1;
                }
            }
            if( state->pieces[sq - 15] == PAWN &&
                state->color[sq - 15] == by_color){
                return 1;
            }
        }
    }
    else{
        if(SQ2ROW(sq) < 7){
            if(SQ2COL(sq) < 7){
                if( state->pieces[sq + 17] == PAWN &&
                    state->color[sq + 17] == by_color){
                    return 1;
                }
            }
            if( state->pieces[sq + 15] == PAWN &&
                state->color[sq + 15] == by_color){
                return 1;
            }
        }        
    }

    //Caballo
    for(int dir = 0; dir < 8; dir++ ){
        from_square = sq + move_directions[KNIGHT][dir];
        if (IS_VALID(from_square) && state->pieces[from_square] == KNIGHT &&
            state->color[from_square] == by_color){
            return 1;
        }
    }
    

    //Rey
    for(int dir = 0; dir < 8; dir++ ){
        from_square = sq + move_directions[KING][dir];
        if (IS_VALID(from_square) && state->pieces[from_square] == KING &&
            state->color[from_square] == by_color){
            return 1;
        }
    }   
    uint8_t straight_dirs[] = {DIR_N, DIR_E, DIR_S, DIR_W};

    for(int dir_n = 0; dir_n < 4; dir_n++){
        uint8_t from_square = sq + straight_dirs[dir_n];

        while (IS_VALID(from_square)){
            if (state->pieces[from_square] != PIECE_EMPTY){
                if ((state->color[from_square] == by_color) 
                &&  (state->pieces[from_square] == ROOK || state->pieces[from_square] == QUEEN ) )
                    return 1;
                else{
                    break;
                }
            }
            from_square = from_square + straight_dirs[dir_n];
        }
        
    }

    uint8_t diag_dirs[] = {DIR_NE, DIR_SE, DIR_SW, DIR_NW};

    for(int dir_n = 0; dir_n < 4; dir_n++){
        uint8_t from_square = sq + diag_dirs[dir_n];

        while (IS_VALID(from_square)){
            if (state->pieces[from_square] != PIECE_EMPTY){
                if ((state->color[from_square] == by_color) 
                &&  (state->pieces[from_square] == QUEEN || state->pieces[from_square] == BISHOP ) )
                    return 1;
                else{
                    break;
                }
            }
            from_square = from_square + diag_dirs[dir_n];
        }
        
    }
    

    return 0;
}


