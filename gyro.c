/*
 * gyro.c
 *
 * Created: 3/29/2017 1:29:11 PM
 *  Author: marwa079
 */ 


//Code for the Gyro-sensor


#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdint.h>
//#include <util/delay.h>
//#include <math.h>
#include <stdbool.h>
#include "spi.h"

const uint8_t SPI_gyro = 0;


//ss to pb0
void ss_to_low_gyro(void)
{
	PORTB = (0<<PORTB0);
}


void ss_to_high_gyro(void)
{
	PORTB = (1<<PORTB0);
}



bool Activate_gyro(void)
{
	bool AD_ready = false;
	uint8_t result = 0;
	
	
	ss_to_low_gyro();
	cli();
	result = spi_send(0b10010100);
	sei();
	ss_to_high_gyro();
	
	//check last bit
	AD_ready = (result & (0<<7));
	
	return AD_ready;
}

