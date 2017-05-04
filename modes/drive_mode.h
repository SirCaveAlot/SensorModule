﻿/*
 * drive_mode.h
 *
 * Created: 4/25/2017 8:57:28 AM
 *  Author: marwa079
 */ 


#ifndef DRIVE_MODE_H_
#define DRIVE_MODE_H_


void Drive_mode(void);

bool Send_all_values( uint8_t gyro_val, uint16_t LIDAR_val , uint8_t module_choice);

bool Check_mode_change(char curr_steering_mode);
#endif /* DRIVE_MODE_H_ */