#include "graphics.h"

#include <stdio.h>

#define USING_NAMESPACE_P8G
#include "../../lib/p8g/p8g.h"

#include "../game/game.h"



Font chess_font;


//[rank][file]
/*
char piece_in_location[8][8] =
{
{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}
};
*/

char piece_in_location[8][8] =
{
{'T', 'M', 'V', 'L', 'W', 'V', 'M', 'T'},
{'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O'},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o'},
{'t', 'm', 'v', 'w', 'l', 'v', 'm', 't'}

};
void init_graphics(){
  chess_font = loadFont("chess_font.ttf");
}




void draw_board() {
  rectMode(CORNERS);
  colorMode(RGB);

  led_color board_colors[8][8];

  get_led_colors(board_colors);

  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      led_color color = board_colors[rank][file];
      fill(color.r, color.g, color.b);
      rect(file*SIZE/8, (7 - rank)*SIZE/8, (file+1)*SIZE/8, (8 - rank)*SIZE/8);
    }
  }
}


void draw_pieces() {
  char piece_str[2];
  piece_str[1] = 0;
  textSize(SIZE/10);
  stroke(255);
  strokeWeight(0);
  fill(0);
  textFont(chess_font);
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      if(piece_in_location[rank][file]){
        piece_str[0] = piece_in_location[rank][file];
        if(piece_str[0] > 'A' && piece_str[0] < 'Z'){
          piece_str[0] += 32;
          strokeWeight(2);
          stroke(0);
          fill(255);
          text(piece_str, (file + 0.1)*SIZE/8, ((7.9 - rank))*SIZE/8);
        }
        else{
          strokeWeight(0);
          stroke(0);
          fill(0);
          text(piece_str, (file + 0.1)*SIZE/8, ((7.9 - rank))*SIZE/8);
        }
      }
    }
  }
}



char piece_stack[64];
int piece_stack_pointer = 0;


void mousePressed() {
    int file = 8*mouseX/SIZE;
    int rank = 8 - 8*mouseY/SIZE;
    

    //Se apreto el trash
    if(mouseX > SIZE){
      if(piece_stack_pointer > 0){
        piece_stack_pointer--;
      }
      return;
    }

    //Se levanto una pieza sin pieza en mano
    if(piece_in_location[rank][file] && piece_stack_pointer == 0){
      
      if(piece_in_location[rank][file] != 'X'){
        //Al menos que sea una pieza invalida,
        //en ese caso no la agarro
        piece_stack[piece_stack_pointer] = piece_in_location[rank][file];
        piece_stack_pointer++;
      }

      piece_in_location[rank][file] = 0;
    }

    //Se levanto una pieza con una pieza en mano (se descarta la que se acaba de levantar)
    else if(piece_in_location[rank][file] && piece_stack_pointer != 0){
      piece_stack[piece_stack_pointer] = piece_in_location[rank][file];
      piece_stack_pointer++;
      piece_in_location[rank][file] = 0;

    }

    //Se apoyo una pieza que se tenia en la mano en un lugar vacio
    else if(!piece_in_location[rank][file] && piece_stack_pointer != 0){
      piece_in_location[rank][file] = piece_stack[piece_stack_pointer-1];
      piece_stack_pointer--;
    }

    //Se apoyo una pieza nueva en el tablero (no deberia pasar)
    else{
      piece_in_location[rank][file] = 'X';
    }


}

void draw_trash(){
  fill(200, 100, 20);
  strokeWeight(10);
  stroke(220, 80, 20);
  rect(SIZE+5, 5, SIZE+SIZE/6-5, SIZE-5);
}


char piece_str[2] = {0,0};
void update_graphics(){
    background(0);
    draw_board();
    draw_pieces();
    draw_trash();
    if(piece_stack_pointer > 0){
      textSize(SIZE/12);
      piece_str[0] = piece_stack[piece_stack_pointer-1];
      if(piece_str[0] > 'A' && piece_str[0] < 'Z'){
        piece_str[0] += 32;
        strokeWeight(2);
        stroke(0);
        fill(255);
        text(piece_str, mouseX-0.25*SIZE/8, mouseY+0.25*SIZE/8);
      }
      else{
        strokeWeight(0);
        stroke(0);
        fill(0);
        text(piece_str, mouseX-0.25*SIZE/8, mouseY+0.25*SIZE/8);
      }
    }
}

void get_sensors_from_graphics(char sensor_data[8]){
  for (int rank = 0; rank < 8; rank++) {
    sensor_data[rank] = 0;
    for (int file = 0; file < 8; file++) {
        sensor_data[rank] |= (piece_in_location[rank][file]!=0 ? 1 : 0) << file;
    }
  }
}


void draw() {

    game_fsm();


    update_graphics();


}

void start_graphics(){
    init_graphics();

    set_sensor_reader((sensor_reader_t)get_sensors_from_graphics);
    
    game_reset();
    

    run(SIZE+SIZE/6, SIZE, "Test chess");
}

void keyPressed() {}
void keyReleased() {}
void mouseMoved() {}
void mouseReleased() {}
void mouseWheel(float delta) {
    delta = delta;
}


