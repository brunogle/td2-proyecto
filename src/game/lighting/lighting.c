#include "lighting.h"
#include "../engine/engine.h"
#include "../movement.h"
#include <stdint.h>


//Array que asocia un ID de color a una 3-tuple (led_color)
led_color color_from_id[6]= {WHITE_COLOR, BLACK_COLOR, VALID_COLOR, LIFTED_COLOR, MISSING_COLOR, INVALID_COLOR};


//Estado de la iluminacion del tablero
char lighting_state = LIGHTING_IDLE_STATE;

//Array donde se escribe la salida
led_color * output_array = (led_color *)0;


//Setter para el array de salida
void lighting_set_output(led_color led_output_array[8][8]){
    output_array = (led_color *)led_output_array;
}


//Cambia un elemento de una casilla del array de salida
void set_color(int rank, int file, char color_id){
    output_array[rank*8 + file] = color_from_id[color_id];
}


/////////////////////////////////////////////
//                                         //
//   FUNCIONES QUE PINTAN CASILLAS         //
//                                         //
/////////////////////////////////////////////

//Pinta casillas de blanco o negro
void paint_board() {
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      set_color(rank, file, (rank&1) == (file&1) ? BLACK_ID : WHITE_ID);
    }
  }
}

//Pinta donde hay lugars validos a donde mover
int paint_valid_moves(uint8_t square_lifted, move_t * valid_moves, int total_valid_moves){

    int lifted_piece_valid_moves = 0;

    total_valid_moves = engine_list_moves(valid_moves);
    for(int i = 0; i < total_valid_moves; i++){
        if(valid_moves[i].from == square_lifted){
            set_color(SQ2ROW(valid_moves[i].to), SQ2COL(valid_moves[i].to), VALID_ID);
            lifted_piece_valid_moves ++;
        }
    }
    return lifted_piece_valid_moves;
}

//Pinta discrepancias entre el estado en memoria del tablero y el leido
char paint_differences(){
    char sensor_state[8];
    get_sensors(sensor_state);
    char board_ok = 1;

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if((sensor_state[rank] & (1 << file)) && engine_get_piece(COORD2SQ(rank, file)) == PIECE_EMPTY){
                set_color(rank, file, MISSING_ID);
                board_ok = 0;
            }
            else if(!(sensor_state[rank] & (1 << file)) && engine_get_piece(COORD2SQ(rank, file)) != PIECE_EMPTY){
                set_color(rank, file, INVALID_ID);
                board_ok = 0;
            }
        }
    }
    return board_ok;
}

//Pinta discrepancias entre el estado en memoria del tablero y la captura esperada
char paint_capture(int _rank, int _file){
    char sensor_state[8];
    get_sensors(sensor_state);
    char board_ok = 1;

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if(_rank == rank && _file == file)
                continue;
            if((sensor_state[rank] & (1 << file)) && engine_get_piece(COORD2SQ(rank, file)) == PIECE_EMPTY){
                set_color(rank, file, MISSING_ID);
                board_ok = 0;
            }
            else if(!(sensor_state[rank] & (1 << file)) && engine_get_piece(COORD2SQ(rank, file)) != PIECE_EMPTY){
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

void lighting_set_state(char state){
    lighting_state = state;
}

void lighting_piece_lifted_square(uint8_t square){
    square_lifted_lighting = square;
}

void set_valid_moves(move_t * moves, int total_valid_moves){
    valid_moves_lighting = moves;
    total_valid_moves_lighting = total_valid_moves;
}

void lighting_refresh(){
    switch (lighting_state) {
        case LIGHTING_IDLE_STATE:
            paint_board();
        break;

        case LIGHTING_LIFTED_STATE:
            paint_board();
            set_color(SQ2ROW(square_lifted_lighting), SQ2COL(square_lifted_lighting), LIFTED_ID);
            paint_valid_moves(square_lifted_lighting, valid_moves_lighting, total_valid_moves_lighting);
        break;

        case LIGHTING_ERROR_STATE:
            paint_board();
            paint_differences();
        break;

        case LIGHTING_CAPTURE_STATE:
            paint_board();
            paint_capture(SQ2ROW(square_lifted_lighting), SQ2COL(square_lifted_lighting));
        break;
    }
}
