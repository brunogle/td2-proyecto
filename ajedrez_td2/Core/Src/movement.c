#include "movement.h"
#include "engine/board.h"
#include "engine/engine.h"
#include "lighting/lighting.h"

char previous_sensor_state[8];
char new_sensor_state[8];

void (*get_sensors)(char[8]);

extern uint8_t reed_data[8];

extern game_state_t engine_game_state;

piece_change_t get_last_sensor_change(){

    //get_sensors(new_sensor_state);


    piece_change_t ret = {0, NONE};

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            char new_state = (reed_data[rank]&(1<<file)) != 0;
            char old_state = (previous_sensor_state[rank]&(1<<file)) != 0;
            if(new_state != old_state){
                ret.square_affected = COORD2SQ(rank, file);
                ret.piece_action = new_state ? PLACED : REMOVED;
                for(int i = 0; i < 8; i++){
                    previous_sensor_state[i] = reed_data[i];
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
    //char sensor_state[8];
    //get_sensors(sensor_state);
    char board_ok = 1;

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if((reed_data[rank] & (1 << file)) && engine_get_piece(COORD2SQ(rank, file)) == PIECE_EMPTY){
                board_ok = 0;
            }
            else if(!(reed_data[rank] & (1 << file)) && engine_get_piece(COORD2SQ(rank, file)) != PIECE_EMPTY){
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

int total_valid_moves = 0;
move_t valid_moves[255] = {0};

color_t cpu_player = COLOR_EMPTY;

move_t cpu_movement;

char capturing = 0;

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
            total_valid_moves = engine_list_moves(valid_moves, 1);



            lifted_piece_valid_moves = get_lifted_moves(square_lifted, valid_moves, total_valid_moves);

            if(lifted_piece_valid_moves == 0){
                movement_state = ERROR_STATE;
                lighting_set_state(LIGHTING_ERROR_STATE);
            }
            else{
                movement_state = PIECE_LIFTED_STATE;
                lighting_piece_lifted_square(square_lifted);
                lighting_set_valid_moves(valid_moves, total_valid_moves);
                lighting_set_state(LIGHTING_LIFTED_STATE);                
            }
        }
        else if(piece_change.piece_action == PLACED){
            movement_state = ERROR_STATE;
            lighting_set_state(LIGHTING_ERROR_STATE);
        }
        break;
    
    case PIECE_LIFTED_STATE:
        if(piece_change.piece_action == PLACED){
            move_t move_played;
            move_played.from = square_lifted;
            move_played.to = piece_change.square_affected;
            char move_ok = engine_move_piece(move_played);
            if(capturing){
                if(cpu_player == COLOR_EMPTY){
                    movement_state = WAIT_STATE;
                    //printf("Eval: %d\n", engine_negamax_seach(engine_game_state, 1, MIN_EVAL, MAX_EVAL));
                    lighting_set_state(LIGHTING_IDLE_STATE);
                }
                else{
                    movement_state = CPU_THINKING_STATE;
                    lighting_set_state(LIGHTING_CPU_THINKING_STATE);
                }
                capturing = 0;
            }
            if(move_ok){
                total_valid_moves = engine_list_moves(valid_moves, 1);
                if(total_valid_moves == 0){
                    lighting_set_winner(1 - engine_game_state.side_to_move);
                    lighting_set_state(LIGHTING_GAME_FINISHED_STATE);
                    movement_state = GAME_FINISHED_STATE;
                }
                else{
                    if(cpu_player == COLOR_EMPTY){
                        movement_state = WAIT_STATE;
                        //printf("Eval: %d\n", engine_negamax_seach(engine_game_state, 1, MIN_EVAL, MAX_EVAL));
                        lighting_set_state(LIGHTING_IDLE_STATE);
                    }
                    else{
                        movement_state = CPU_THINKING_STATE;
                        lighting_set_state(LIGHTING_CPU_THINKING_STATE);
                    }
                }
            }
            else{
                movement_state = ERROR_STATE;
                lighting_set_state(LIGHTING_ERROR_STATE);
            }
        }
        else if(piece_change.piece_action == REMOVED){

            total_valid_moves = engine_list_moves(valid_moves, 1);

            char captured_piece_is_valid = 0;
            for(int i = 0; i < total_valid_moves; i++){
                if(valid_moves[i].from == square_lifted && valid_moves[i].to == piece_change.square_affected){
                    captured_piece_is_valid = 1;
                    lighting_piece_lifted_square(piece_change.square_affected);
                    lighting_set_state(LIGHTING_CAPTURE_STATE); 
                    engine_move_piece(valid_moves[i]);
                    capturing = 1;
                    break;
                }
            }

            if(!captured_piece_is_valid){
                movement_state = ERROR_STATE;
                lighting_set_state(LIGHTING_ERROR_STATE);
            }
        }
        
        break;

    case ERROR_STATE:
        board_ok = is_board_ok();

        if(board_ok){
            movement_state = WAIT_STATE;
            lighting_set_state(LIGHTING_IDLE_STATE);
        }
        break;

    case CPU_THINKING_STATE:
        cpu_movement = engine_search();
        movement_state = CPU_LIFT_FROM_STATE;
        lighting_set_cpu_movement(cpu_movement.from, cpu_movement.to);
        lighting_set_state(LIGHTING_CPU_LIFT_FROM_STATE);
        /*
        if(engine_finished()){

        }
        */
        break;

    case CPU_LIFT_FROM_STATE:

        //Player must lift cpu piece
        if(piece_change.piece_action == REMOVED){
            if(piece_change.square_affected == cpu_movement.from){
                if(engine_get_piece(cpu_movement.to) != PIECE_EMPTY){
                    movement_state = CPU_LIFT_CAPTURED_STATE;
                    lighting_set_state(LIGHTING_CPU_LIFT_CAPTURED_STATE);
                }
                else{
                    lighting_set_state(LIGHTING_CPU_PLACE_TO_STATE);
                    movement_state = CPU_PLACE_TO_STATE;
                }
            }
        }
        break;

    case CPU_LIFT_CAPTURED_STATE:
        //Player must lift cpu piece
        if(piece_change.piece_action == REMOVED){
            if(piece_change.square_affected == cpu_movement.to){
                movement_state = CPU_PLACE_TO_STATE;
                lighting_set_state(LIGHTING_CPU_PLACE_TO_STATE);
            }
        }
        break;

    case CPU_PLACE_TO_STATE:
        //Player must lift cpu piece
        if(piece_change.piece_action == PLACED){
            if(piece_change.square_affected == cpu_movement.to){
                movement_state = WAIT_STATE;
                engine_move_piece(cpu_movement);
                lighting_set_state(LIGHTING_IDLE_STATE);
            }
        }
        break;

    case GAME_FINISHED_STATE:
        lighting_set_state(GAME_FINISHED_STATE);
    break;

    default:
        break;
    }
}
