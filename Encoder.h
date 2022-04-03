#ifndef ENCODER_H_
#define ENCODER_H_

#include "motor_control.h" 

extern int isMoving;

void initEncoder(void);
void setMotion_and_Speed(enum directions_t Direction, uint8_t speed);
void forward_distance(int distance);
void turn_degree(enum directions_t Direction,int angle);

#endif
