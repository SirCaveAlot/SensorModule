/*
 * straight_mode.c
 *
 * Created: 4/3/2017 3:10:10 PM
 *  Author: marwa079
 */ 

#include "../sensors/analog_sensors.h"
#include "../spi.h"
#include "../global_definitions.h"

char straight_send(uint8_t module_nr)
{
	char mode = 'S';
	
	ss_to_low(module_nr);
	
	for(uint8_t i = 0 ; i < 8 ; ++i)
	{
	    mode = spi_send(_analog_sensor_values[i]);	
	}
	
	
	ss_to_high(module_nr);
	
	return mode;
	
}


void straight_mode(void)
{
	
	read_analog_sensors(0b11111011);
	_comm_mode = straight_send(comm_ss_port_);
	_steering_mode = straight_send(steering_ss_port_);
	
}