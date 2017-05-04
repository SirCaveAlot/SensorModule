/*
 * delay.c
 *
 * Created: 5/3/2017 1:18:39 PM
 *  Author: marwa079
 */ 

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdbool.h>

volatile uint8_t tot_overflow_send;

ISR(TIMER2_OVF_vect)
{
	++tot_overflow_send;
}

//one count+1 takes 4.34 *10^-6 seconds = 4.34 us
void Setup_timer2(void)
{
	//prescaler on /64
	TCCR2B = (1<<CS22) ;
	TCNT2 = 0;
	//enable overflow interrupts
	//TIMSK1 |= (1<<TOIE1);
}


//activates or deactivates Overflow interrupts for the timer
void Activate_or_deactivate_counter2(bool activate_count)
{
	
	if(activate_count)
	{
		tot_overflow_send = 0;
		TIMSK2 |= (1<<TOIE2);
		TCNT2 = 0;
	}
	else
	{
		TIMSK2 &= ~(1<<TOIE2);
		tot_overflow_send = 0;
		TCNT2 = 0;
	}
}

//works to maximum of 4.34us*256 = 1111 us = 1.111 ms
//max input 1111
void delay(uint8_t u_seconds)
{
	
	uint32_t selected_counter = (u_seconds / 4.34);
	Activate_or_deactivate_counter2(true);
	sei();
	while( TCNT2 <= selected_counter); //&& ((uint16_t)(u_seconds / 255) <= tot_overflow_send)));
	Activate_or_deactivate_counter2(false);
	
	
}



void delay_100_ms(void)
{
	for(uint8_t i = 0 ; i < 100 ; ++i)
	{
		delay( 1000);
	}
}