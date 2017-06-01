/*
 * SensorModule.h
 *
 * Created: 3/28/2017 9:41:33 AM
 *  Author: Marcus Wallin, marwa079
 */ 


#ifndef SENSOR_MODULE_H_
#define SENSOR_MODULE_H_

void interrupt_setup(void);

void SPI_setup(void);

extern volatile char _steering_mode;

void Mode_loop(void);

#endif /* SENSOR_MODULE_H_ */

