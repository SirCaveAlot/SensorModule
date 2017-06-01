/*
 * gyro.h
 *
 * Created: 3/29/2017 1:05:12 PM
 *  Author: Marcus Wallin, marwa079
 */ 




#ifndef GYRO_H_
#define GYRO_H_

#include <stdbool.h>
#include <stdlib.h>

bool Activate_gyro(void);

bool Check_EOC_bit(void);

uint16_t Read_gyro(void);

void Start_conversion_gyro(void);

bool Start_gyro(void);

uint16_t Get_angular_velocity(void);


#endif /* GYRO_H_ */