/*
 * laser_mode.c
 *
 * Created: 4/3/2017 3:30:13 PM
 *  Author: marwa079
 */ 

#include "laser_mode.h"
#include "../UART.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


volatile uint8_t tot_overflow;
bool magnet_detected;
uint16_t counter_val; 


void Setup_timer(void)
{
	//prescaler on /8
	TCCR0B = (1<<CS01) ;
	//TCNT0 = 0;
	//enable overflow interrupts
	//TIMSK1 |= (1<<TOIE1);
}


//TCNT1 stores Timer/Counter values

//TIFR TOV1 is high if T/C1 has overflown


ISR(TIMER1_OVF_vect)
{
	++tot_overflow;
}


//interrupt vector for HAll sensor
ISR(INT1_vect)
{
	uint8_t high_byte = TCNT1H;
	counter_val = (high_byte << 8);
	counter_val |= TCNT1L;
	
	TCNT1H = 0;
	TCNT1L = 0;
}