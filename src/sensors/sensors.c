#include "sensors.h"
#include "../graphics.h"


void get_sensors(char * sensor_data){
  for (int rank = 0; rank < 8; rank++) {
    sensor_data[rank] = 0;
    for (int file = 0; file < 8; file++) {
        sensor_data[rank] |= (piece_in_location[rank][file]!=0 ? 1 : 0) << file;
    }
  }
}
