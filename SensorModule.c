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




volatile uint8_t ADread;

uint8_t sensor_values[8];

volatile uint8_t curr_sensor;





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
	
	//DDRB = 0xFF;
	
	sei();
	
    while(1);
}





ISR(INT0_vect,ISR_NOBLOCK)
{
	//uint8_t nr = 3;
	
	//read_single_analog(2);
	//read_analog_sensors(nr);
	
	//DDRD |= (1<<DDD3);
	//PORTD |= (1<<PORTD3);
	
	bool gyro_active = false;
	
	
		gyro_active = Activate_gyro();
	//}while(!gyro_active);
	
	if(gyro_active)
	{
		DDRD |= (1<<DDD3);
		PORTD |= (1<<PORTD3); 
	}
	
	
	
}


//interrupt vector for the ADC

ISR(ADC_vect){
	

	sensor_values[curr_sensor] = ADCH;
	
	ADCSRA = 0x00;
	
	ADread = 1;
	
	sei();
}




void SPI_setup(void)
{
	
	//ss signal to gyro
	DDRB = (1<<DDB0);

	//add more values later

	PORTB = (1<<PORTB0) | (1<<PORTB4);
}



void Overall_setup(void)
{
	
	
	
}

//reads analog sensors from A0-AN. 
//nr_of_sensors is the number of inputs connected.
//requires a global int named ADread.

void read_analog_sensors(uint8_t nr_of_sensors)
{
	
	
	uint8_t localADread = 0;
	
	//temporary code
	++ADMUX; 
	++ADMUX;
	//---
	
	PORTB = 0xFF;
	
	for (int i = 0 ; i < nr_of_sensors ; ++i)
	{
		ADCSRA = 0xEC;
		
		do
		{
			cli();
			localADread = ADread;
			sei();
			
		} while (localADread == 0);
		
		
		++ADMUX;
		++curr_sensor;
		
		ADread = 0;
		localADread = 0;
	
	
	}
	
	PORTB = 0x00;
	
	//display_values(nr_of_sensors);
	//reset values
	curr_sensor = 0;
	ADMUX = (1<<ADLAR);
	
}


void read_single_analog(uint8_t sensor_nr)
{
      
	  uint8_t localADread = 0;
	  
	  ADMUX = ADMUX + sensor_nr;
	  ADCSRA = 0xEC;
	  
	  do
	  {
		  cli();
		  localADread = ADread;
		  sei();
		  
	  } while (localADread == 0);
	  
	  
	 
	  ADread = 0;
	  localADread = 0;
	  ADMUX = (1<<ADLAR);
	  
}




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


void sensor_values_zero(void)
{
	for(uint8_t i = 0 ; i < 8 ; ++i)
	{
		
		sensor_values[i] = 0;
		
	}
	
}
