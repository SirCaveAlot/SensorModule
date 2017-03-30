/*
 * analog_sensors.c
 *
 * Created: 3/30/2017 3:52:27 PM
 *  Author: marwa079
 */ 

#include "analog_sensors.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/interrupt.h>

volatile uint8_t curr_sensor;

volatile uint8_t ADread = 0;

uint8_t _analog_sensor_values[8];

ISR(ADC_vect){
	

	_analog_sensor_values[curr_sensor] = ADCH;
	
	ADCSRA = 0x00;
	
	ADread = 1;
	
	sei();
}





void read_analog_sensors(uint8_t nr_of_sensors, uint8_t curr_sensor)
{
	
	
	
	
	for (int sensor = 0 ; sensor < nr_of_sensors ; ++sensor)
	{
		
		read_single_analog(sensor);
		/*ADCSRA = 0xEC;
		
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
		*/
		
	}
	
	
	
	//display_values(nr_of_sensors);
	//reset values
	curr_sensor = 0;
	
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



void sensor_values_zero(void)
{
	for(uint8_t i = 0 ; i < 8 ; ++i)
	{
		
		_analog_sensor_values[i] = 0;
		
	}
	
}
