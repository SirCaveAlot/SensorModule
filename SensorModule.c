/*
 * SensorModule.c
 *
 * Created: 3/28/2017 9:41:33 AM
 *  Author: marwa079
 */ 


#define 	F_CPU   128000UL

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include <math.h>
#include <stdbool.h>

#include "SensorModule.h"
#include "spi.h"
#include "gyro.h"
#include "analog_sensors.h"





uint8_t sensor_values[8];

volatile uint8_t curr_sensor;

bool _gyro_activated = false;

uint8_t value = 0;	

int main(void)
{   
	//set up for global variables
	/*sensor_values_zero();
	ADread = 0;
	curr_sensor = 0;
	*/
	SPI_setup();
	interrupt_setup();
	spi_init(0,1,0,1,0);
	DDRD |= (1<<DDD3);

	
	//DDRB = 0xFF;
	
	sei();
	
    while(1);
}





ISR(INT0_vect)
{
	
	//read_single_analog(2);
	//read_analog_sensors(nr);
	
	
	if(_gyro_activated)
	{
		
		value = Get_angular_velocity();
		PORTD |= (0<<PORTD3);
	}
	else
	{
		Start_gyro();
		_gyro_activated = true;
		PORTD |= (1<<PORTD3);
	    
		
	}
	
	
	
}


//interrupt vector for the ADC

//sets up the SPI properly and activates the Gyro.
void SPI_setup(void)
{
	
	//ss signal to gyro
	DDRB = (1<<DDB0);

	//add more values later

	PORTB = (1<<PORTB0) | (1<<PORTB4);
	Activate_gyro();
}



void Overall_setup(void)
{
	
	
	
}

//reads analog sensors from A0-AN. 
//nr_of_sensors is the number of inputs connected.
//requires a global int named ADread.





double IR_conversion(uint8_t val)
{
	
	return (55.25 * exp(-0.05762 * val)) + (14.2 * exp(-0.009759 * val));
	
	
}










void display_values(uint8_t nr)
{
	for(int i = 0 ; i < nr ; ++i)
	{
		
		PORTB = sensor_values[i];
		
		_delay_ms (1000);
		//delay_fcn(30000);
		
	}
	
	
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


