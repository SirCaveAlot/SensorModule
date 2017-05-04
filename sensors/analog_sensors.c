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
#include <stdbool.h>

static volatile uint8_t _curr_sensor;

volatile uint8_t _AD_read = 0;

volatile uint8_t _analog_sensor_values[8];




ISR(ADC_vect){
	

	_analog_sensor_values[_curr_sensor] = ADCH;
	
	ADCSRA = 0x00;
	
	_AD_read = 1;
	
	sei();
}





void read_analog_sensors(uint8_t sensor_bits)
{
	
	for (int sensor = 0 ; sensor < 8 ; ++sensor)
	{
		_curr_sensor = sensor;
		
		if((1<<sensor) == (sensor_bits & (1<<sensor)))
		{
		    read_single_analog(sensor);	
		}
		
	}
	
	_curr_sensor = 0;
	
}



void read_single_analog(uint8_t sensor_nr)
{
	
	uint8_t localADread = 0;
	
	ADMUX = ADMUX + sensor_nr;
	ADCSRA = 0xEC;
	
	do
	{
		cli();
		localADread = _AD_read;
		sei();
		
	} while (localADread == 0);
	
	
	
	_AD_read = 0;
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


char _wheel_color_left;
char _wheel_color_right;


bool Check_color_change(uint8_t wheel_sensor_val , uint8_t black_threshold, uint8_t white_threshold, bool left_wheel)
{
	char curr_color; 
	cli();
	if(left_wheel)
	{
		curr_color = _wheel_color_left;
	}
	else
	{
		curr_color = _wheel_color_right;
	}
	
	
	if(wheel_sensor_val > black_threshold)
	{
		//black  color
		curr_color = 'B';
		
	}
	else if (wheel_sensor_val < white_threshold)
	{
		//white color
		curr_color = 'W';
		
	}
	
	bool return_bool = false;
	
	if(left_wheel)
	{
		return_bool  = (curr_color !=  _wheel_color_left);
		 _wheel_color_left = curr_color;	
	}
	else
	{
		return_bool  = (curr_color !=  _wheel_color_right);
		 _wheel_color_right = curr_color;	
	}
	sei();
   
	return return_bool;
	
}


uint8_t leftwheel_changed = 0;
uint8_t rightwheel_changed = 0;

void color_check_wheel(bool left_wheel)
{
	if(left_wheel)
	{
		PORTD &= ~(1<<PORTD5);
		_curr_sensor = 5;
		read_single_analog(5);
		if(Check_color_change(_analog_sensor_values[5], 200, 100, true))
		{
			++leftwheel_changed;
		}
	}
	else
	{
		PORTD |= (1<<PORTD5);
		_curr_sensor = 4;
		read_single_analog(4);
	    if(Check_color_change(_analog_sensor_values[4], 200, 100, false))
	    {
		    ++rightwheel_changed;
	    }
	
	}
	
}



