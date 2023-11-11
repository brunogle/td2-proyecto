#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

/* row identifiers */

#define ROW_1   ( A1 >> 4 )
#define ROW_2   ( A2 >> 4 )
#define ROW_3   ( A3 >> 4 )
#define ROW_4   ( A4 >> 4 )
#define ROW_5   ( A5 >> 4 )
#define ROW_6   ( A6 >> 4 )
#define ROW_7   ( A7 >> 4 )
#define ROW_8   ( A8 >> 4 )

/* column identifiers */

#define COL_A  ( A1 & 7 )
#define COL_B  ( B1 & 7 )
#define COL_C  ( C1 & 7 )
#define COL_D  ( D1 & 7 )
#define COL_E  ( E1 & 7 )
#define COL_F  ( F1 & 7 )
#define COL_G  ( G1 & 7 )
#define COL_H  ( H1 & 7 )

#define COORD2SQ(row, col) (((row) << 4) + (col))
#define SQ2ROW(sq) ((sq) >> 4)
#define SQ2COL(sq) ((sq) & 7)
#define IS_VALID(sq) !((sq) & 0x88)

#define STARTFEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"


/*
Copiado del engine CPW-engine
*/

typedef enum enum_piece {
    KING,
    QUEEN,
    ROOK,
    BISHOP,
    KNIGHT,
    PAWN,
    PIECE_EMPTY
} piece_t;

typedef enum enum_color { //El orden importa
    WHITE,
    BLACK,
    COLOR_EMPTY
} color_t;

typedef enum enum_squares {
    A1=0  , B1, C1, D1, E1, F1, G1, H1,
    A2=16 , B2, C2, D2, E2, F2, G2, H2,
    A3=32 , B3, C3, D3, E3, F3, G3, H3,
    A4=48 , B4, C4, D4, E4, F4, G4, H4,
    A5=64 , B5, C5, D5, E5, F5, G5, H5,
    A6=80 , B6, C6, D6, E6, F6, G6, H6,
    A7=96 , B7, C7, D7, E7, F7, G7, H7,
    A8=112, B8, C8, D8, E8, F8, G8, H8
} square_t;

typedef enum enum_castle_flags{
    WHITE_KINGSIDE = 1,
    WHITE_QUEENSIDE = 2,
    BLACK_KINGSIDE = 4,
    BLACK_QUEENSIDE = 8
} castle_flag_t;

/*
Representacion "0x88" del tablero.
https://www.chessprogramming.org/0x88
*/
typedef struct game_state{
    piece_t pieces[128];
    color_t color[128];
    color_t side_to_move;
    char castle_flags;
} game_state_t;


/*
Estructura para representar movimientos
"from" y "to" en 0 representa movimiento no valido. Esto
es retornado por funciones que retornan move_t

Cuando el miembro promotion es distinto de 0 significa que el movimiento requiere una selección de promoción
*/
typedef struct move{
    uint8_t from;
    uint8_t to;
    uint8_t promotion;
} move_t;


int clear_game_state(game_state_t * state);
int place_piece(game_state_t * state, piece_t piece, color_t color, uint8_t square);
int remove_piece(game_state_t * state, uint8_t square);



#endif
