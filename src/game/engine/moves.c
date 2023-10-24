#include "moves.h"
#include "board.h"
#include "../../pc/pc.h"


/*
Primer indice del array determina el tipo de pieza, el sub-array da todas las direcciones posibles en la que la pieza puede moverse.
Esta informacion, junto con piece_can_slide[5] determina como se mueve cada pieza
*/
int8_t move_directions[5][8] = {
    { DIR_N, DIR_E, DIR_S, DIR_W, DIR_NE, DIR_SE, DIR_SW, DIR_NW}, //King move directions
    { DIR_N, DIR_E, DIR_S, DIR_W, DIR_NE, DIR_SE, DIR_SW, DIR_NW}, //Queen move directions
    { DIR_N, DIR_E, DIR_S, DIR_W}, //Rook move directions
    { DIR_NE, DIR_SE, DIR_SW, DIR_NW}, //Bishop move directions
    { DIR_NNW, DIR_NNE, DIR_NEE, DIR_SEE, DIR_SSE, DIR_SSW, DIR_SWW, DIR_NWW} //Knight move directions
};

//Array auxiliar para iterar sobre move_directions (K, Q, R, B, N)
uint8_t total_move_directions[5] = {8, 8, 4, 4, 8};

//Determina si la pieza se desliza (K, Q, R, B, N)
uint8_t piece_can_slide[5] = {0, 1, 1, 1, 0};


int make_move(game_state_t * state, move_t move, uint8_t safe){

    if(move.from == move.to)
        return 0;

    if(safe){
        uint8_t move_ok = 0;
        move_t moves[256];
        uint8_t nmoves = generate_moves(state, moves);
        for(uint8_t i = 0; i < nmoves; i++){
            if(moves[i].from == move.from && moves[i].to == move.to){
                move_ok = 1;
            }
        }
        if(!move_ok){
            return 0;
        }
    }

    state->side_to_move = (color_t)(1 - (uint8_t)state->side_to_move);

    if(state->pieces[move.to] != PIECE_EMPTY)   
        remove_piece(state, move.to);

    if(move.promotion != 0)
    {
        place_piece(state, check_promotion(), state->color[move.from], move.to);
    }
    else
    {
        place_piece(state, state->pieces[move.from], state->color[move.from], move.to);
    }

    remove_piece(state, move.from);
    return 1;
}

/*
    Devuelve la pieza seleccionada para promoción.
*/
piece_t check_promotion()
{
    return get_promotion();
}

/*
Retorna todos los movimientos pseudo-legales para un estado.
Los movimientos pseudo-legales son los legales incluyendo los que dejan
al rey en jaque (que segun las reglas no es un movimiento legal)
*/
int generate_moves(game_state_t * state, move_t * moves){

    uint16_t n_moves = 0;

    for(int square = 0; square < 128; square++){

        uint8_t piece_type = state->pieces[square];

        if(piece_type == PIECE_EMPTY) //Lugar vacio
            continue;

        if(state->color[square] != state->side_to_move) //No corresponde al color que mueve en el turno
            continue;

        /*
        Los peones se mueven muy distino al resto de las piezas, lo tomo como caso separado
        */
        else if(piece_type == PAWN){
            if((state->side_to_move) == (WHITE)){//Peon blanco
                if(IS_VALID(square + DIR_N)){
                    if(state->pieces[square + DIR_N] == PIECE_EMPTY){
                        moves[n_moves].from = square;
                        moves[n_moves].to = square + DIR_N;
                        if(SQ2ROW(moves[n_moves].to) == ROW_8)
                            moves[n_moves].promotion = 1;
                        n_moves++;
                        if(SQ2ROW(square) == ROW_2 && state->pieces[square + DIR_NN] == PIECE_EMPTY){//Peon blanco sin mover con lugar vacio
                            moves[n_moves].from = square;
                            moves[n_moves].to = square + DIR_NN;   
                            n_moves++;
                        }
                    }
                }
                /*
                    Capturas de peon blanco
                */
                if(IS_VALID(square + DIR_NE)){
                    if(state->color[square + DIR_NE] == BLACK){
                        moves[n_moves].from = square;
                        moves[n_moves].to = square + DIR_NE;
                        if(SQ2ROW(moves[n_moves].to) == ROW_8)
                            moves[n_moves].promotion = 1;
                        n_moves++;    
                    }
                }
                if(IS_VALID(square + DIR_NW)){
                    if(state->color[square + DIR_NW] == BLACK){
                        moves[n_moves].from = square;
                        moves[n_moves].to = square + DIR_NW;
                        if(SQ2ROW(moves[n_moves].to) == ROW_8)
                            moves[n_moves].promotion = 1;
                        n_moves++;    
                    }
                }
            } 
            else{//Peon negro
                if(IS_VALID(square + DIR_S)){
                    if(state->pieces[square + DIR_S] == PIECE_EMPTY){
                        moves[n_moves].from = square;
                        moves[n_moves].to = square + DIR_S;
                        if(SQ2ROW(moves[n_moves].to) == ROW_1)
                            moves[n_moves].promotion = 1;
                        n_moves++;
                        if(SQ2ROW(square) == ROW_7 && state->pieces[square + DIR_SS] == PIECE_EMPTY){ //Peon negro sin mover
                            moves[n_moves].from = square;
                            moves[n_moves].to = square + DIR_SS;
                            n_moves++;                    
                        }
                    }
                }
                /*
                    Capturas de peon negro
                */
                if(IS_VALID(square + DIR_SE)){
                    if(state->color[square + DIR_SE] == WHITE){
                        moves[n_moves].from = square;
                        moves[n_moves].to = square + DIR_SE;
                        if(SQ2ROW(moves[n_moves].to) == ROW_1)
                            moves[n_moves].promotion = 1;
                        n_moves++;    
                    }
                }
                if(IS_VALID(square + DIR_SW)){
                    if(state->color[square + DIR_SW] == WHITE){
                        moves[n_moves].from = square;
                        moves[n_moves].to = square + DIR_SW;
                        if(SQ2ROW(moves[n_moves].to) == ROW_1)
                            moves[n_moves].promotion = 1;
                        n_moves++;    
                    }
                }
            }
        }
        /*
        En caso de que sea rey, reina, torre, alfil o caballo
        */
       
        else{
            //Itero sobre todas las direcciones posibles
            for(int8_t direction_num = 0; direction_num < total_move_directions[piece_type]; direction_num++){

                uint8_t new_square = square + move_directions[piece_type][direction_num];

                //Asumo que la pieza se puede deslizar, luego salgo del loop si no se puede
                while(IS_VALID(new_square)){

                    if(state->pieces[new_square] == PIECE_EMPTY){ //Lugar vacio, es un movimiento valido
                        moves[n_moves].from = square;
                        moves[n_moves].to = new_square;
                        n_moves++;
                    }
                    else{
                        if(state->color[new_square] != state->side_to_move){ //Lugar con pieza enemiga, se puede capturar
                            moves[n_moves].from = square;
                            moves[n_moves].to = new_square;
                            n_moves++;    
                        }
                        break; //Si estaba deslizando, ya no puede avanzar mas.                        

                    }
                    //Si no se puede deslizar, dejo de checkear esta direccion
                    if(!piece_can_slide[piece_type]){
                        break;
                    }
                    //Si se puede deslizar, avanzo un cuadrado en la misma direccion
                    new_square += move_directions[piece_type][direction_num];
                }
            }
        }
    }
    return n_moves;
}
