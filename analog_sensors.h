/*
 * analog_sensors.h
 *
 * Created: 3/30/2017 3:52:46 PM
 *  Author: marwa079
 */ 


#ifndef ANALOG_SENSORS_H_
#define ANALOG_SENSORS_H_

#include <stdint.h>

void read_analog_sensors(uint8_t nr_of_sensors, uint8_t curr_sensor);

void read_single_analog(uint8_t sensor_nr);

void sensor_values_zero(void);

#endif /* ANALOG_SENSORS_H_ */