/*
 * test_mode.c
 *
 * Created: 4/3/2017 11:35:48 AM
 *  Author: marwa079
 */ 
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#define 	F_CPU   14745600UL
#include "test_mode.h"
#include "../global_definitions.h"
#include "../communication/spi.h"
#include "../SensorModule.h"
#include <stdbool.h>
#include <util/delay.h>

extern char distance_data;

char spi_send_to_module(uint8_t val, uint8_t module_choice)
{
	char return_mode;
	
	ss_to_low(module_choice);
	
	return_mode = spi_send(val);
	
	ss_to_high(module_choice);
	
	return return_mode;
}

/*
void Send_0_to_9(void)
{
	test_spi_send(0x00, comm_ss_port_);
	//Check_mode_change(curr_steering_mode);
	// test_spi_send(0xFF, steering_ss_port_);
	_delay_us(100);
	PORTD |= (1<<PORTD4);
	test_spi_send(1, comm_ss_port_);
	_delay_us(100);
	test_spi_send(2, comm_ss_port_);
	_delay_us(100);
	test_spi_send(3, comm_ss_port_);
	_delay_us(100);
	test_spi_send(4, comm_ss_port_);
	_delay_us(100);
	test_spi_send(5, comm_ss_port_);
	_delay_us(100);
	test_spi_send(6, comm_ss_port_);
	_delay_us(100);
	test_spi_send(7, comm_ss_port_);
	_delay_us(100);
	test_spi_send(8, comm_ss_port_);
	uint8_t curr_steering_mode = test_spi_send(9, comm_ss_port_);
	Check_mode_change(curr_steering_mode);


	_delay_ms(100);

}
*/


