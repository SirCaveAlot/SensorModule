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

static volatile uint8_t curr_sensor;

volatile uint8_t ADread = 0;

volatile uint8_t _analog_sensor_values[8];

ISR(ADC_vect){
	

	_analog_sensor_values[curr_sensor] = ADCH;
	
	ADCSRA = 0x00;
	
	ADread = 1;
	
	sei();
}





void read_analog_sensors(uint8_t sensor_bits)
{
	
	for (int sensor = 0 ; sensor < 8 ; ++sensor)
	{
		curr_sensor = sensor;
		
		if((1<<sensor) == (sensor_bits & (1<<sensor)))
		{
		    read_single_analog(sensor);	
		}
		
	}
	
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
