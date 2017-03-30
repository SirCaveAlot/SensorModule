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

void Activate_gyro(void);

void ss_to_high_gyro(void);

bool Check_EOC_bit(void);

uint16_t Read_gyro(void);

void Start_conversion_gyro(void);

void Start_gyro(void);

uint8_t Get_angular_velocity(void);


#endif /* GYRO_H_ */