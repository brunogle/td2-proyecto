#include "game.h"
#include "engine/engine.h"
#include "movement.h"
#include "lighting/lighting.h"

sensor_reader_t sensor_reader = (sensor_reader_t)0;

void set_sensor_reader(sensor_reader_t reader){
    sensor_reader = reader;
    get_sensors = reader;
}

void get_led_colors(led_color board_led_colors[8][8]){

    for(int file = 0; file < 8; file++){
        for(int rank = 0; rank < 8; rank++){
            board_led_colors[rank][file].r = color_from_id[square_colors[rank][file]][0];
            board_led_colors[rank][file].g = color_from_id[square_colors[rank][file]][1];
            board_led_colors[rank][file].b = color_from_id[square_colors[rank][file]][2];
        }
    }

}

void game_fsm(){

    movement_fsm();

    refresh_board_coloring();

}

void game_reset(){

    reset_game();

}