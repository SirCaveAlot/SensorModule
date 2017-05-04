//header file

#ifndef SENSOR_MODULE_H_
#define SENSOR_MODULE_H_

#include <stdbool.h>
#include <stdint.h>
#define manual_mode_ 1<<2

void interrupt_setup(void);

extern bool max_speed_bool;
//void read_analog_sensors(uint8_t nr_of_sensors);

void delay_fcn(int clkcycles);

//void read_single_analog(uint8_t sensor_nr);

//void sensor_values_zero(void);

void display_values(uint8_t nr);



void SPI_setup(void);

bool Send_value_both_modules(uint8_t send_value);



extern volatile char _steering_mode;
void Mode_loop(void);

#endif /* SENSOR_MODULE_H_ */

