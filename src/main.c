#include <stdio.h>
#include <stdint.h>

#define USING_NAMESPACE_P8G
#include "../lib/p8g/p8g.h"
#include "graphics.h"


#include "engine/board.h"
#include "engine/moves.h"
#include "engine/interface.h"
#include "engine/engine.h"
#include "movement.h"
#include "lighting/lighting.h"
#define MAX_MOVES 100








/*
Esta funcion se llama 60 veces por segundo
*/
void draw() {

    movement_fsm();

    refresh_board_coloring();

    update_graphics();


}

int main(){
    init_graphics();

    
    reset_game();
    get_last_sensor_change();
    run(SIZE+SIZE/6, SIZE, "Test chess");
}





