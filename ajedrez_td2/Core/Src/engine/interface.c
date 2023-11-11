#include "engine/interface.h"
#include "engine/moves.h"

#include <stdio.h>


int load_fen(game_state_t * state, const char * fen) {

    clear_game_state(state);

    char * f = (char *)fen;

    char col = 0;
    char row = 7;

    do {
        switch( f[0] ) {
        case 'K':
            place_piece(state, KING, WHITE, COORD2SQ(row, col));
            col++;
            break;
        case 'Q':
            place_piece(state, QUEEN, WHITE, COORD2SQ(row, col));
            col++;
            break;
        case 'R':
            place_piece(state, ROOK, WHITE, COORD2SQ(row, col));
            col++;
            break;
        case 'B':
            place_piece(state, BISHOP, WHITE, COORD2SQ(row, col));
            col++;
            break;
        case 'N':
            place_piece(state, KNIGHT, WHITE, COORD2SQ(row, col));
            col++;
            break;
        case 'P':
            place_piece(state, PAWN, WHITE, COORD2SQ(row, col));
            col++;
            break;
        case 'k':
            place_piece(state, KING, BLACK, COORD2SQ(row, col));
            col++;
            break;
        case 'q':
            place_piece(state, QUEEN, BLACK, COORD2SQ(row, col));
            col++;
            break;
        case 'r':
            place_piece(state, ROOK, BLACK, COORD2SQ(row, col));
            col++;
            break;
        case 'b':
            place_piece(state, BISHOP, BLACK, COORD2SQ(row, col));
            col++;
            break;
        case 'n':
            place_piece(state, KNIGHT, BLACK, COORD2SQ(row, col));
            col++;
            break;
        case 'p':
            place_piece(state, PAWN, BLACK, COORD2SQ(row, col));
            col++;
            break;
        case '/':
            row--;
            col=0;
            break;
        case '1':
            col+=1;
            break;
        case '2':
            col+=2;
            break;
        case '3':
            col+=3;
            break;
        case '4':
            col+=4;
            break;
        case '5':
            col+=5;
            break;
        case '6':
            col+=6;
            break;
        case '7':
            col+=7;
            break;
        case '8':
            col+=8;
            break;
        };

        f++;
    } while ( f[0] != ' ' );

    f++;

    if (f[0]=='w') {
        state->side_to_move = WHITE;
    } else {
        state->side_to_move = BLACK;
    }

    f+=2;

    do {
        // switch( f[0] ) {
        // case 'K':
        //     b.castle |= CASTLE_WK;
        //     break;
        // case 'Q':
        //     b.castle |= CASTLE_WQ;
        //     break;
        // case 'k':
        //     b.castle |= CASTLE_BK;
        //     break;
        // case 'q':
        //     b.castle |= CASTLE_BQ;
        //     break;
        // }

        f++;
    } while (f[0] != ' ' );

    //b.hash ^= zobrist.castling[b.castle];

    f++;

    // if (f[0] != '-') {
    //     b.ep = convert_a_0x88(f);
    //     b.hash ^= zobrist.ep[b.ep];
    // }

    do {
        f++;
    }
    while (f[0] != ' ' );
    f++;
	// int ply = 0;
	// int converted;
    // converted = sscanf(f, "%d", &ply);
	// b.ply = (unsigned char) ply;

    // b.rep_index = 0;
    // b.rep_stack[b.rep_index] = b.hash;

    return 1;
}



void display_board(game_state_t state) {

    uint8_t sq;

    char parray[3][7] = { {'K','Q','R','B','N','P'},
        {'k','q','r','b','n','p'},
        { 0 , 0 , 0 , 0 , 0,  0, '.'}
    };

    printf("   a b c d e f g h\n\n");

    for (int8_t row=7; row>=0; row--) {

        printf("%d ", row+1);

        for (uint8_t col=0; col<8; col++) {
            sq = COORD2SQ(row, col);
            printf(" %c",parray[state.color[sq]][state.pieces[sq]] );
        }

        printf("  %d\n", row+1);

    }

    printf("\n   a b c d e f g h\n\n");
}



move_t str_to_move(char * str) {
    move_t move;
    move.from = 0;
    move.to = 0;

    uint8_t from;
    uint8_t to;

    from = str_to_square(str);
    if(from == 255){
        return move;
    }
    to = str_to_square(str+2);
    if(to == 255){
        return move;
    }

    move.from = from;
    move.to = to;
    
    return move;


}

uint8_t str_to_square(char * str){
    uint8_t square;
    if(str[0] >= 'a' && str[0] <= 'h'){
        square = str[0] - 'a';
    }
    else{
        return 255;
    }
    if(str[1] >= '1' && str[1] <= '8'){
        square += (str[1] - '1')<<4; 
    }
    else{
        return 255;
    }
    return square;
}


//returns new pointer
int move_to_str(move_t move, char * str) {
    square_to_string(move.from, str);
    square_to_string(move.to, str+2);
    return 1;
}

int square_to_string(uint8_t sq, char * str) {
    str[0] = SQ2COL(sq) + 'a';
    str[1] = SQ2ROW(sq) + '1';
    str[2] = 0;
    return 0;
}
