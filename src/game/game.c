#include "game.h"
#include "engine/engine.h"
#include "movement.h"
#include "lighting/lighting.h"

sensor_reader_t sensor_reader = (sensor_reader_t)0;

void game_set_sensor_reader(sensor_reader_t reader){
    sensor_reader = reader;
    get_sensors = reader;
}

void game_set_led_output_array(led_color output_array[8][8]){
    lighting_set_output(output_array);
}


void game_fsm(){

    movement_fsm();

    lighting_refresh();

}

void game_reset(){

    engine_reset();

}