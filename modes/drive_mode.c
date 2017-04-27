/*
 * drive_mode.c
 *
 * Created: 4/25/2017 8:57:07 AM
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
		
	}
	else
	{
		TIMSK2 &= ~(1<<TOIE2);
		tot_overflow_send = 0;
		
	}
}