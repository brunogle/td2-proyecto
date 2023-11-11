/*
Header que funciona como interfaz entre el resto del prgrama y el engine.
Solo se deben llamar funciones de este header de afuera de la carpeta "engine"
*/


#ifndef ENGINE_H
#define ENGINE_H


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
int engine_list_moves(move_t * moves);


char engine_get_piece(uint8_t square);

#endif

