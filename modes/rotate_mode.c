/*
 * rotate_mode.c
 *
 * Created: 4/3/2017 2:36:17 PM
 *  Author: marwa079
 */ 

#include "../global_definitions.h"
#include "rotate_mode.h"
#include "../spi.h"
#include "../sensors/gyro.h"


char rotate_send(uint8_t ang_vel, uint8_t module_nr)
{
	
    ss_to_low(module_nr);
	
	
	char mode = spi_send(ang_vel);
	
	
	ss_to_high(module_nr);
    	
	return mode;
	
}


void rotate_mode()
{
	uint8_t ang_vel = Get_angular_velocity();
	_comm_mode = rotate_send(ang_vel, comm_ss_port_);
	_steering_mode = rotate_send(ang_vel, steering_ss_port_);
	
}


