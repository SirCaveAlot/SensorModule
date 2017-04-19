﻿/*
 * laser_mode.c
 *
 * Created: 4/3/2017 3:30:13 PM
 *  Author: marwa079
 */ 

#include "laser_mode.h"
#include "../UART.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint16_t tot_overflow;

volatile uint8_t magnet_count = 0; 
volatile uint16_t distance_vector[1000];
volatile uint16_t angle_vector[1000];
volatile uint16_t UART_data;

volatile uint16_t last_full_rotate_time = 0;
volatile uint16_t vector_count = 0;
volatile bool MSByte = false;


void Setup_timer(void)
{
	//prescaler on /8
	TCCR0B = (1<<CS01) ;
	TCNT0 = 0;
	//enable overflow interrupts
	//TIMSK1 |= (1<<TOIE1);
}


//TCNT1 stores Timer/Counter values

//TIFR TOV1 is high if T/C1 has overflown

//----------------------------------------------------------------------
//interrupts
ISR(TIMER0_OVF_vect)
{
	++tot_overflow;
}

//counts every time a magnet activates the interrupt 

ISR(INT2_vect)
{
	last_full_rotate_time = tot_overflow; 
	++magnet_count;
	tot_overflow = 0;
}


ISR(USART0_RX_vect)
{
	
	
	
	if(get_LIDAR_16bit_data())
	{
		distance_vector[vector_count] = UART_data;
		angle_vector[vector_count] = Calculate_angle();
		++vector_count;
	}
	
	
	
}

//-----------------------------------------------------------------------------



uint16_t Calculate_angle(void)
{
	//muliply this with any number to get angle
	return (((double)tot_overflow / last_full_rotate_time) * 1000);
}



//activates or deactivates Overflow interrupts for the timer
void Activate_or_deactivate_counter(bool activate_count)
{
	
	if(activate_count)
	{
	    TIMSK0 |= (1<<TOIE0);	
	    
	}
	else
	{
		TIMSK0 &= ~(1<<TOIE0);
		tot_overflow = 0;
		
	}
}






void Laser_speed_mode(void)
{
	cli();
	PORTB &= ~(1<<PORTB3);
    
	Activate_or_deactivate_counter(true);
	sei();
	while(!Steady_LIDAR_ang_vel()) {};
	
	cli();
	PORTB = (1<<PORTB3);
	Enable_USART_interrupt();
	//change depending on letter
	USART_Transmit('T');
	sei();
	
	while(vector_count < 1000);
    
	cli();
	USART_Transmit('L');
	Disable_USART_interrupt();
	Activate_or_deactivate_counter(false);
	
	vector_count = 0;
	magnet_count = 0;
	sei();
}





bool Steady_LIDAR_ang_vel(void)
{
	
	return (magnet_count >= 2);
	
}


//sets MSByte false if UART receives a 0.
//When the next interrupt comes, the MSByte will be set to true.
bool get_LIDAR_16bit_data(void)
{
	uint16_t temp_data = UDR0;
	
	bool temp_bool = false;
	
	if(temp_data == 0xFF)
	{
		MSByte = false;
	}
	else if((temp_data != 0xFF) && !MSByte)
	{
		UART_data = temp_data;
		MSByte = true;
	}
	else if(MSByte)
	{
		temp_data = (temp_data << 8);
		UART_data |= temp_data;
		UART_data = (UART_data >> 1);
	    temp_bool = true;
	}
	
	return temp_bool;
	//maybe transmit further or do something with the data
	
}

