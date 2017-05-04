﻿/*
 * analog_sensors.h
 *
 * Created: 3/30/2017 3:52:46 PM
 *  Author: marwa079
 */ 


#ifndef ANALOG_SENSORS_H_
#define ANALOG_SENSORS_H_

#include <stdint.h>
#include <stdbool.h>

void read_analog_sensors(uint8_t sensor_bits);

void read_single_analog(uint8_t sensor_nr);

void sensor_values_zero(void);

void color_check_wheel(bool left_wheel);
bool Check_color_change(uint8_t wheel_sensor_val , uint8_t black_threshold, uint8_t white_threshold, bool left_wheel);

extern uint8_t rightwheel_changed;
extern uint8_t leftwheel_changed;
extern volatile uint8_t _analog_sensor_values[8]; 

#endif /* ANALOG_SENSORS_H_ */