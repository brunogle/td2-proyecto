#include "lighting.h"
#include "../engine/engine.h"
#include "../sensors/sensors.h"
#include <stdint.h>


char lighting_state = LIGHTING_IDLE_STATE;
int square_colors[8][8];

float color_from_id[6][4] = {WHITE_COLOR, BLACK_COLOR, VALID_COLOR, LIFTED_COLOR, MISSING_COLOR, INVALID_COLOR};


void set_color(int rank, int file, char color_id){
    square_colors[rank][file] = color_id;
}

void color_board() {
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      set_color(rank, file, (rank&1) == (file&1) ? BLACK_ID : WHITE_ID);
    }
  }
}

int color_valid_moves(uint8_t square_lifted, move_t * valid_moves, int total_valid_moves){

    int lifted_piece_valid_moves = 0;

    total_valid_moves = list_moves(valid_moves);
    for(int i = 0; i < total_valid_moves; i++){
        if(valid_moves[i].from == square_lifted){
            set_color(SQ2ROW(valid_moves[i].to), SQ2COL(valid_moves[i].to), VALID_ID);
            lifted_piece_valid_moves ++;
        }
    }
    return lifted_piece_valid_moves;
}


char color_differences(){
    char sensor_state[8];
    get_sensors(sensor_state);
    char board_ok = 1;

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if((sensor_state[rank] & (1 << file)) && get_piece(COORD2SQ(rank, file)) == PIECE_EMPTY){
                set_color(rank, file, MISSING_ID);
                board_ok = 0;
            }
            else if(!(sensor_state[rank] & (1 << file)) && get_piece(COORD2SQ(rank, file)) != PIECE_EMPTY){
                set_color(rank, file, INVALID_ID);
                board_ok = 0;
            }
        }
    }
    return board_ok;
}

uint8_t square_lifted_lighting;
move_t * valid_moves_lighting;
int total_valid_moves_lighting;

void set_lighting_state(char state){
    lighting_state = state;
}

void set_square_lifted(uint8_t square){
    square_lifted_lighting = square;
}

void set_valid_moves(move_t * moves, int total_valid_moves){
    valid_moves_lighting = moves;
    total_valid_moves_lighting = total_valid_moves;
}

void refresh_board_coloring(){
    switch (lighting_state) {
        case LIGHTING_IDLE_STATE:
            color_board();
        break;

        case LIGHTING_LIFTED_STATE:
            color_board();
            set_color(SQ2ROW(square_lifted_lighting), SQ2COL(square_lifted_lighting), LIFTED_ID);
            color_valid_moves(square_lifted_lighting, valid_moves_lighting, total_valid_moves_lighting);
        break;

        case LIGHTING_ERROR_STATE:
            color_board();
            color_differences();
        break;
    }
}
