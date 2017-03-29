//header file

#ifndef SENSOR_MODULE_H_
#define SENSOR_MODULE_H_


void interrupt_setup(void);


void read_analog_sensors(uint8_t nr_of_sensors);

void delay_fcn(int clkcycles);

void read_single_analog(uint8_t sensor_nr);

void sensor_values_zero(void);

void display_values(uint8_t nr);

void SPI_setup(void);





#endif /* SENSOR_MODULE_H_ */

