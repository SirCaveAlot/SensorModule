/*
 * test_mode.c
 *
 * Created: 4/3/2017 11:35:48 AM
 *  Author: marwa079
 */ 
#include <stdint.h>

#include "test_mode.h"
#include "../global_definitions.h"


char test_send_to_comm(uint8_t val)
{
	char return_mode;
	
	ss_to_low(comm_ss_port_);
	
	return_mode = spi_send(val);
	
	ss_to_high(comm_ss_port_);
	
	return return_mode;
}