#include "pc.h"
#include "graphics.h"
#include "../game/game.h"

#include <stdio.h>

void pc_main(){

    //Le digo a "game" que funcion utilizar para leer los sensores
    game_set_sensor_reader((sensor_reader_t)graphics_get_sensors);

    //Le digo a "game" donde guardar los colores de los LEDs
    game_set_led_output_array(graphics_led_colors);


    game_reset();

    //Una vez que se ejecuta esto, p8g comienza a ejecutar en un bucle draw()
    graphics_start(); 
}

/*
Esta funcion se llama 60 veces por segundo por p8g (libreria incluida en graphics.h)
*/
void draw() {
    game_fsm(); //Ejecuto una vuelta del FSM del juego

    graphics_draw_all(); //Actualizo los graficos en el tablero
}

piece_t get_promotion()
{
    printf("Ingrese numero de pieza para seleccionar la promocion:\n");
    printf("Caballo: %d\n", KNIGHT);
    printf("Alfil: %d\n", BISHOP);
    printf("Reina: %d\n", QUEEN);

    unsigned int prom;

    printf("Seleccion: ");
    scanf("%u", &prom);
    
    return (piece_t) prom;
}