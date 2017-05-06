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

#include "../global_definitions.h"
#include "../delay.h"
#include "../sensors/analog_sensors.h"
#include "../sensors/gyro.h"
#include "../SensorModule.h"
#include "../modes/laser_mode.h"
#include "test_mode.h"
#include "../UART.h"
#include "drive_mode.h"









bool Check_mode_change(char curr_steering_mode)
{
	bool mode_changed = (curr_steering_mode != _steering_mode);
	
	 _steering_mode = curr_steering_mode;
	
	if(mode_changed)
	{
		PORTD &= ~((1<<PORTD4) | (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7));
	}
	
	return mode_changed;
	
}


//Sends all values to both modules and returns true if mode has changed during the send
//
bool Send_all_values( uint8_t gyro_val, uint16_t LIDAR_val , uint8_t module_choice)
{
	
    //------SEND STARTBYTES-------------------------------
    if(Send_value_both_modules(0xFF))
    {
	    return true;
    }

    
	if(Send_value_both_modules(0xFF))
	{
		return true;
	}
	
	//------SEND ANALOGS---------------------------------
	//except wheels
	for(uint8_t i = 0 ; i < 4 ; ++i)
	{
		
		if(Send_value_both_modules(_analog_sensor_values[i]))
		{
			return true;
		}
		
	}
	
	if(Check_color_change(_analog_sensor_values[4], 220 , 100, false))
	{
		++rightwheel_changed;
	}
	
	if(Send_value_both_modules(rightwheel_changed))
	{
		return true;
	}
	rightwheel_changed = 0;
	
	
	if(Check_color_change(_analog_sensor_values[5], 220 , 100, true))
	{
		++leftwheel_changed;
	}
	
	if(Send_value_both_modules(leftwheel_changed))
	{
		return true;
	}
	leftwheel_changed = 0;
	
	//------SEND GYRO---------------------------------
	
	if(Send_value_both_modules(gyro_val))
	{
		return true;
	}
	
	//------SEND LIDAR---------------------------------
	uint8_t LIDAR_low_byte = (uint8_t) LIDAR_val;
	uint8_t LIDAR_high_byte = (uint8_t) (LIDAR_val>>8);
	if(Send_value_both_modules(LIDAR_high_byte))
	{
		return true;
	}
	
	
	if(Send_value_both_modules(LIDAR_low_byte))
	{
		return true;
	}
	

	
	return false;
}



void Drive_mode(void)
{
	Activate_or_deactivate_counter2(true);
	
	read_analog_sensors(0x3F);
	uint8_t gyro_value = Get_angular_velocity();
	Enable_USART_interrupt();
	
	bool left_wheel = true;
	
	//send with frequency of 50 hz works
	while(tot_overflow_send < 17)
	{
		
		color_check_wheel(left_wheel);
		left_wheel = !left_wheel;
		for(uint16_t i = 0 ; i < 2000 ; ++i);
	}
	Disable_USART_interrupt();
	Activate_or_deactivate_counter2(false);
	PORTD |= (1<<PORTD6);
	
	Send_all_values(gyro_value, LIDAR_distance, comm_ss_port_);
	
		
	
	
	PORTD &= ~(1<<PORTD6);
	
}



bool Send_value_both_modules(uint8_t send_value)
{
	
	char curr_steering_mode = spi_send_to_module(send_value, comm_ss_port_);
	if(Check_mode_change(curr_steering_mode))
	{
		delay(40);
		return true;
	}
	
	
	spi_send_to_module(send_value, steering_ss_port_);
	delay(40);
	
	return false;
}


