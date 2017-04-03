/*
 * rotate_mode.h
 *
 * Created: 4/3/2017 2:36:32 PM
 *  Author: marwa079
 */ 


#ifndef ROTATE_MODE_H_
#define ROTATE_MODE_H_

#include <stdint.h>

char rotate_send(uint8_t ang_vel, uint8_t module_nr);

void rotate_mode();


#endif /* ROTATE_MODE_H_ */