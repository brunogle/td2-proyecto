#ifndef DRAWINGS_H
#define DRAWINGS_H

#define SIZE 600


extern char piece_in_location[8][8];

void get_sensors(char * sensor_data);

void update_graphics();

void init_graphics();



void keyPressed();
void keyReleased();
void mouseMoved();
void mousePressed();
void mouseReleased();
void mouseWheel(float delta);

#endif

