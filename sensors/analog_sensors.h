/*
 * analog_sensors.h
 *
 * Created: 3/30/2017 3:52:46 PM
 *  Author: marwa079
 */ 


#ifndef ANALOG_SENSORS_H_
#define ANALOG_SENSORS_H_

#include <stdint.h>

void read_analog_sensors(uint8_t sensor_bits);

void read_single_analog(uint8_t sensor_nr);

void sensor_values_zero(void);

extern volatile uint8_t _analog_sensor_values[8]; 

#endif /* ANALOG_SENSORS_H_ */