/*
 * test_mode.c
 *
 * Created: 4/3/2017 11:35:48 AM
 *  Author: marwa079
 */ 
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>

#include "test_mode.h"
#include "../global_definitions.h"
#include "../spi.h"
#include "../SensorModule.h"
#include <stdbool.h>

extern char UART_data;

char test_spi_send(uint8_t val, uint8_t module_choice)
{
	char return_mode;
	
	ss_to_low(module_choice);
	
	return_mode = spi_send(val);
	
	ss_to_high(module_choice);
	
	return return_mode;
}







void test_Laser_max_freq(void)
{
	
	
	
	while(max_speed_bool)
	{
		
        PORTB = UART_data;		
		
	}
	
	
}