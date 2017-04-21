//header file

#ifndef SENSOR_MODULE_H_
#define SENSOR_MODULE_H_

#include <stdbool.h>

#define manual_mode_ 1<<2

void interrupt_setup(void);

extern bool max_speed_bool;
//void read_analog_sensors(uint8_t nr_of_sensors);

void delay_fcn(int clkcycles);

//void read_single_analog(uint8_t sensor_nr);

//void sensor_values_zero(void);

void display_values(uint8_t nr);

void Send_all_values( uint8_t gyro_val, uint16_t LIDAR_val , uint8_t module_choice);

void SPI_setup(void);





#endif /* SENSOR_MODULE_H_ */

