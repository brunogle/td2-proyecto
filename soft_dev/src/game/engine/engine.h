/*
Header que funciona como interfaz entre el resto del prgrama y el engine.
Solo se deben llamar funciones de este header de afuera de la carpeta "engine"
*/


#ifndef ENGINE_H
#define ENGINE_H

#define PAWN_SCORE 1
#define BISHOP_SCORE 3
#define KNIGHT_SCORE 3
#define ROOK_SCORE 5
#define QUEEN_SCORE 9

#define MIN_EVAL -10000
#define MAX_EVAL 10000



#include "moves.h"
#include "board.h"

/*
Reinicia el estado del tablero
*/
void engine_reset();

/*
Aplica un movimiento
*/
char engine_move_piece(move_t move);

/*
Devuelve todos los movimientos LEGALES atravez del array "moves".
TODO: Hacer que solo devuelva los legales
*/
int engine_list_moves(move_t * moves, char only_legal);


char engine_get_piece(uint8_t square);

int engine_negamax_seach(game_state_t game_state, int depth, int alpha, int beta);

move_t engine_search();

#endif

