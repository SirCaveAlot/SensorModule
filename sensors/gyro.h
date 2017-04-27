/*
 * gyro.h
 *
 * Created: 3/29/2017 1:05:12 PM
 *  Author: marwa079
 */ 




#ifndef GYRO_H_
#define GYRO_H_

#include <stdbool.h>
#include <stdlib.h>

//#define _gyro_ss 0b00000001

bool Activate_gyro(void);

bool Check_EOC_bit(void);

uint8_t Read_gyro(void);

void Start_conversion_gyro(void);

bool Start_gyro(void);

uint8_t Get_angular_velocity(void);


#endif /* GYRO_H_ */