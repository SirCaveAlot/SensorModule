/*
 * SensorModule.c
 *
 * Created: 3/28/2017 9:41:33 AM
 *  Author: marwa079
 */ 


#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "SensorModule.h"


volatile int val;

int main(void)
{
	interrupt_setup();
	
	DDRB = 0xFF;
	
	sei();
	
    while(1);
}



//EIMSK sets INT0 as interrupt
//EICRA activates interrupts on rising edge
//ADMUX sets the highest bits in the AD converter to ADCH
void interrupt_setup(void)
{
	 
	 EIMSK = 1<<INT0;
	 EICRA = (1<<ISC01) | (1<<ISC00);
	 ADMUX = (1<<ADLAR);
	 
}






ISR(INT0_vect, ISR_NOBLOCK)
{
	int nr = 1;
	
	read_analog_sensors(nr);
	
}


ISR(ADC_vect){
	
	PORTB = ADCH;
	ADCSRA = 0x00;
	
	
	
	val = 1;
	
	sei();
}



//reads analog sensors from A0-AN. 
//nr_of_sensors is the number of inputs connected.
//requires a global int named val.

void read_analog_sensors(int nr_of_sensors)
{
	
	
	int localval = 0;
	
	ADMUX = (1<<ADLAR);
	
	
	for (int i = 0 ; i < nr_of_sensors ; ++i)
	{
		ADCSRA = 0xEC;
		
		do
		{
			cli();
			localval = val;
			sei();
			
		} while (localval == 0);
		
		
		++ADMUX;
		val = 0;
		localval = 0;
	
		//this is temporary
	    delay_fcn(20000);
	}
}




void delay_fcn(int clkcycles)
{
	
	for(int i = 0 ; i < clkcycles ; ++i)
	{
		
	}
	
}