/*
 * analog_sensors.c
 *
 * Created: 3/30/2017 3:52:27 PM
 *  Author: Marcus Wallin, marwa079
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



//interrupt vector for the ADC
ISR(ADC_vect){
	
	_analog_sensor_values[_curr_sensor] = ADCH;
	
	ADCSRA = 0x00;
	
	_AD_read = 1;
	
	sei();
}


/*
A0: Right IR front
A1: Left IR
A2: Right reflex
A3: Left reflex
A4: Right Wheel
A5: Left Wheel
A6: Forward IR
A6: Right IR back
A7: Left IR back 
*/

//reads all aanalog sensors
void Read_analog_sensors(uint8_t sensor_bits)
{
	
	for (int sensor = 0 ; sensor < 8 ; ++sensor)
	{
		_curr_sensor = sensor;
		
		if((1<<sensor) == (sensor_bits & (1<<sensor)))
		{
		    Read_single_analog(sensor);	
		}
		
	}
	
	_curr_sensor = 0;
	
}


//reads a single analog sensor
void Read_single_analog(uint8_t sensor_nr)
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




char _wheel_color_left;
char _wheel_color_right;

//checks if the colors on the wheels have changed. Returns true if so.
bool Check_color_change(uint8_t wheel_sensor_val , uint8_t black_threshold, 
						uint8_t white_threshold, bool left_wheel)
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
uint8_t line_detected_count = 0;
bool peepz_detected;

//reads left wheel if left_wheel is true. Otherwise right_wheel.
//Also calls for Check_color_change
void Color_check_wheel(bool left_wheel)
{
	if(left_wheel)
	{
		PORTD &= ~(1<<PORTD5);
		_curr_sensor = 5;
		Read_single_analog(5);
		if(Check_color_change(_analog_sensor_values[5], 200, 100, true))
		{
			++leftwheel_changed;
		}
	}
	else
	{
		PORTD |= (1<<PORTD5);
		_curr_sensor = 4;
		Read_single_analog(4);
	    if(Check_color_change(_analog_sensor_values[4], 200, 100, false))
	    {
		    ++rightwheel_changed;
	    }
	
	}
	
}

//Check if robot detect line under left 
//it needs to have detected a black line 6 7 times to set
//peepz_detected to true
void Check_peepz_in_needz(void)
{   
	//left sensor
	_curr_sensor = 3;
	Read_single_analog(3);
	if(_analog_sensor_values[3] > 250)
	{
		++line_detected_count;
	    if (line_detected_count > 6)
	    {
	        peepz_detected = true;      
	    }
	}
	else
	{
		line_detected_count = 0;
	}
}



