/*
 * gyro.h
 *
 * Created: 3/29/2017 1:05:12 PM
 *  Author: marwa079
 */ 

#include <stdbool.h>


#ifndef GYRO_H_
#define GYRO_H_


void ss_to_low_gyro(void);

bool Activate_gyro(void);

void ss_to_high_gyro(void);

#endif /* GYRO_H_ */