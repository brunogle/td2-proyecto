#include "graphics.h"
#include <stdint.h>

//Libreria p8g utilizada para graficos
//https://bernhardfritz.github.io/p8g/
#define USING_NAMESPACE_P8G
#include "../../lib/p8g/p8g.h"


//game.h es la interfaz al sofware central del proyecto
//#include "../game/game.h"


led_color graphics_led_colors[8][8];

/*
Los dibujos de las piecas son caracteres de chess_font.ttf
A continuacion se especifica que caracter corresponde a que pieza
*/

#define KING_CHAR 'l'
#define QUEEN_CHAR 'w'
#define KNIGHT_CHAR 'm'
#define ROOK_CHAR 't'
#define BISHOP_CHAR 'm'
#define PAWN_CHAR 'o'
#define GENERIC_PIECE 'x'

Font chess_font; //chess_font.ttf

typedef struct{
  char piece;
  char color;
} piece_char_t;

#define NO_PIECE (piece_char_t){0,0}

/*
Letras correspondientes a chess_font.ttf
Solo representa lo que se visualiza en pantalla,
puede no corresponder con el estado real del juego.

t/T: Torre
m/M: Caballo
v/V: Alfil
w/W: Reina
l/L: Rey
o/O: Peon

Mayusculas: Blancas
Minusulas: Negras

Para llamar a text() todas son convertidas en minuscula.
El color proviene cambiando fill()

*/
piece_char_t piece_in_location[8][8] =
{
{{ROOK_CHAR, WHITE}, {KNIGHT_CHAR, WHITE}, {BISHOP_CHAR, WHITE}, {KING_CHAR, WHITE},
{QUEEN_CHAR, WHITE}, {BISHOP_CHAR, WHITE}, {KING_CHAR, WHITE}, {ROOK_CHAR, WHITE}},//rank 1

{{PAWN_CHAR, WHITE}, {PAWN_CHAR, WHITE}, {PAWN_CHAR, WHITE}, {PAWN_CHAR, WHITE},
{PAWN_CHAR, WHITE}, {PAWN_CHAR, WHITE}, {PAWN_CHAR, WHITE}, {PAWN_CHAR, WHITE}},//rank 2

{NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE},//rank 3

{NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE},//rank 4

{NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE},//rank 5

{NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE, NO_PIECE},//rank 6


{{PAWN_CHAR, BLACK}, {PAWN_CHAR, BLACK}, {PAWN_CHAR, BLACK}, {PAWN_CHAR, BLACK},
{PAWN_CHAR, BLACK}, {PAWN_CHAR, BLACK}, {PAWN_CHAR, BLACK}, {PAWN_CHAR, BLACK}},//rank 7

{{ROOK_CHAR, BLACK}, {KNIGHT_CHAR, BLACK}, {BISHOP_CHAR, BLACK}, {KING_CHAR, BLACK},
{QUEEN_CHAR, BLACK}, {BISHOP_CHAR, BLACK}, {KING_CHAR, BLACK}, {ROOK_CHAR, BLACK}}//rank 8

};



/*
El cursor actua como un stack FILO.
Cuando se levanta una pieza se agrega al stack Si se deja una pieza sale del stack.
Estas son las variables donde se almacena el stack.
*/
piece_char_t piece_stack[64];
int piece_stack_pointer = 0;





/////////////////////////////////////////////
//                                         //
//  FUNCIONES QUE DIBUJAN EN LA PANTALLA   //
//                                         //
/////////////////////////////////////////////


/*
Dibuja las casillas del tablero
*/
void draw_board() {
  rectMode(CORNERS);
  colorMode(RGB);

  //Obtiene colores de la instancia de game
  //game_get_led_colors(board_colors);

  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {

      fill(graphics_led_colors[rank][file].r,
           graphics_led_colors[rank][file].g,
           graphics_led_colors[rank][file].b);


      rect(file*SIZE/8.0, (7 - rank)*SIZE/8.0, (file+1)*SIZE/8.0, (8 - rank)*SIZE/8.0);

    }
  }


}

/*
Dibuja las piezas en el tablero
*/
void draw_pieces() {

  //text() recibe char*. Por eso necesito armar un string terminado en 0
  char piece_str[2];
  piece_str[1] = '\0';

  textSize(SIZE/10.0);
  stroke(0);
  textFont(chess_font);

  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {

      if(piece_in_location[rank][file].piece != 0){
        //si hay una pieza

        piece_str[0] = piece_in_location[rank][file].piece;

        if(piece_in_location[rank][file].color == WHITE){
          //pieza es blanca.
          strokeWeight(2);
          fill(255);
        }
        else{
          //pieza es negra
          strokeWeight(0);
          fill(0);
        }
        
        text(piece_str, (file + 0.1)*SIZE/8.0, ((7.9 - rank))*SIZE/8.0);

      }
    }
  }
}

/*
A donde van a parar las piezas capturadas
*/
void draw_trash(){
  fill(200, 100, 20);
  stroke(220, 80, 20);
  strokeWeight(10);
  rect(SIZE+5, 5, SIZE+SIZE/6.0-5, SIZE-5);
}


/*
En caso de que se tenga una pieza en la mano (el stack) dibujo la ultima.
*/

void draw_hand(){

  char piece_str[2];
  piece_str[1] = '\0';

  if(piece_stack_pointer > 0){

    textSize(SIZE/12.0);
    stroke(0);

    piece_str[0] = piece_stack[piece_stack_pointer-1].piece;

    if(piece_stack[piece_stack_pointer-1].color == WHITE){
      strokeWeight(2);
      fill(255);
      text(piece_str, mouseX-0.25*SIZE/8.0, mouseY+0.25*SIZE/8.0);
    }
    else{
      strokeWeight(0);
      fill(0);
      text(piece_str, mouseX-0.25*SIZE/8.0, mouseY+0.25*SIZE/8.0);
    }
  }
}

/*
Llama a todas las funciones que dibujan en pantalla en el orden correcto
*/
void graphics_draw_all(){

    background(0);

    draw_board();
    draw_pieces();
    draw_trash();
    draw_hand();

}


/////////////////////////////////////////////
//                                         //
//             EVENT HANDLING              //
//                                         //
/////////////////////////////////////////////


/*
Cuando se presiono el mouse
*/
void mousePressed() {
    int file = 8*mouseX/SIZE;
    int rank = 8 - 8*mouseY/SIZE;
    

    //Si se apreto a la derecha del tablero, se apreto la basura
    if(mouseX > SIZE){
      if(piece_stack_pointer > 0){
        piece_stack_pointer--;
      }
      return;
    }

    //Se levanto una pieza sin pieza en mano, la pongo en el stack y la borro
    if(piece_in_location[rank][file].piece != 0 && piece_stack_pointer == 0){
      
      if(piece_in_location[rank][file].piece != GENERIC_PIECE){
        //Al menos que sea una pieza invalida,
        //en ese caso no la agarro
        piece_stack[piece_stack_pointer] = piece_in_location[rank][file];
        piece_stack_pointer++;
      }

      piece_in_location[rank][file] = NO_PIECE;
    }

    //Se levanto una pieza con una pieza en mano, la pongo en el stack y la borro
    else if(piece_in_location[rank][file].piece != 0 && piece_stack_pointer != 0){
      piece_stack[piece_stack_pointer] = piece_in_location[rank][file];
      piece_stack_pointer++;
      piece_in_location[rank][file] = NO_PIECE;

    }

    //Se apoyo una pieza que se tenia en la mano en un lugar vacio, la saco del stack y la pongo
    else if(piece_in_location[rank][file].piece == 0 && piece_stack_pointer != 0){
      piece_in_location[rank][file] = piece_stack[piece_stack_pointer-1];
      piece_stack_pointer--;
    }

    //Se apoyo una pieza nueva en el tablero (no deberia pasar, es solo para pruebas)
    else{
      piece_in_location[rank][file].piece = GENERIC_PIECE;
    }


}

//Es necesario que esten definidas para que p8g compile
void keyPressed() {}
void keyReleased() {}
void mouseMoved() {}
void mouseReleased() {}
void mouseWheel(float delta) {
    delta = delta;
}


/////////////////////////////////////////////
//                                         //
//   FUNCIONES LLAMADAS EXTERNAMENTE       //
//                                         //
/////////////////////////////////////////////

/*
Esta es la funcion que es pasada como puntero a set_sensor_reader() de game.h
Es llamada internamente por game_fsm() para obtener el estado del tablero.
Esta funcion simula las lecturas de los sensores reed.
Esta funcion devuelve la ocupancia del tablero.
*/
void graphics_get_sensors(uint8_t sensor_data[8]){
  for (int rank = 0; rank < 8; rank++) {
    sensor_data[rank] = 0;
    for (int file = 0; file < 8; file++) {
        sensor_data[rank] |= (piece_in_location[rank][file].piece!=0 ? 1 : 0) << file;
    }
  }
}


/*
Inicia la rutina de p8g
*/
void graphics_start(){
    chess_font = loadFont("chess_font.ttf");

    run(SIZE+SIZE/6, SIZE, "Test chess");
}


