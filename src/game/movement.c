#include "movement.h"
#include "engine/board.h"
#include "engine/engine.h"
#include "lighting/lighting.h"

char previous_sensor_state[8];
char new_sensor_state[8];

void (*get_sensors)(char[8]);

piece_change_t get_last_sensor_change(){

    get_sensors(new_sensor_state);

    piece_change_t ret = {0, NONE};

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            char new_state = (new_sensor_state[rank]&(1<<file)) != 0;
            char old_state = (previous_sensor_state[rank]&(1<<file)) != 0;
            if(new_state != old_state){
                ret.square_affected = COORD2SQ(rank, file);
                ret.piece_action = new_state ? PLACED : REMOVED;
                for(int i = 0; i < 8; i++){
                    previous_sensor_state[i] = new_sensor_state[i];
                }
                return ret;
            }
        }
    }
    return ret;
}


void get_expected_sensor_state(){

}

char movement_state = WAIT_STATE;

uint8_t square_lifted;

char is_board_ok(){
    char sensor_state[8];
    get_sensors(sensor_state);
    char board_ok = 1;

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if((sensor_state[rank] & (1 << file)) && get_piece(COORD2SQ(rank, file)) == PIECE_EMPTY){
                board_ok = 0;
            }
            else if(!(sensor_state[rank] & (1 << file)) && get_piece(COORD2SQ(rank, file)) != PIECE_EMPTY){
                board_ok = 0;
            }
        }
    }
    return board_ok;
}

int get_lifted_moves(uint8_t square_lifted, move_t * valid_moves, int total_valid_moves){

    int lifted_piece_valid_moves = 0;

    for(int i = 0; i < total_valid_moves; i++){
        if(valid_moves[i].from == square_lifted){
            lifted_piece_valid_moves ++;
        }
    }
    return lifted_piece_valid_moves;
}

int total_valid_moves;
move_t valid_moves[255];

void movement_fsm(){

    piece_change_t piece_change = get_last_sensor_change();


    int lifted_piece_valid_moves;
    char board_ok;

    switch (movement_state)
    {
    case WAIT_STATE:
        if(piece_change.piece_action == REMOVED){
            movement_state = PIECE_LIFTED_STATE;
            square_lifted = piece_change.square_affected;
            total_valid_moves = list_moves(valid_moves);
            
            lifted_piece_valid_moves = get_lifted_moves(square_lifted, valid_moves, total_valid_moves);

            if(lifted_piece_valid_moves == 0){
                movement_state = ERROR_STATE;
                set_lighting_state(LIGHTING_ERROR_STATE);
            }
            else{
                movement_state = PIECE_LIFTED_STATE;
                set_square_lifted(square_lifted);
                set_valid_moves(valid_moves, total_valid_moves);
                set_lighting_state(LIGHTING_LIFTED_STATE);                
            }
        }
        else if(piece_change.piece_action == PLACED){
            movement_state = ERROR_STATE;
            set_lighting_state(LIGHTING_ERROR_STATE);
        }
        break;
    
    case PIECE_LIFTED_STATE:
        if(piece_change.piece_action == PLACED){
            move_t move_played;
            move_played.from = square_lifted;
            move_played.to = piece_change.square_affected;
            char move_ok = move_piece(move_played);

            if(move_ok){
                movement_state = WAIT_STATE;
                set_lighting_state(LIGHTING_IDLE_STATE);
            }
            else{
                movement_state = ERROR_STATE;
                set_lighting_state(LIGHTING_ERROR_STATE);
            }
        }
        else if(piece_change.piece_action == REMOVED){

            total_valid_moves = list_moves(valid_moves);

            char captured_piece_is_valid = 0;
            for(int i = 0; i < total_valid_moves; i++){
                if(valid_moves[i].from == square_lifted && valid_moves[i].to == piece_change.square_affected){
                    captured_piece_is_valid = 1;
                    break;
                }
            }

            if(!captured_piece_is_valid){
                movement_state = ERROR_STATE;
                set_lighting_state(LIGHTING_ERROR_STATE);
            }
        }
        
        break;

    case ERROR_STATE:
        board_ok = is_board_ok();

        if(board_ok){
            movement_state = WAIT_STATE;
            set_lighting_state(LIGHTING_IDLE_STATE);
        }
        break;



    default:
        break;
    }
}

