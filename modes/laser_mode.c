﻿/*
 * laser_mode.c
 *
 * Created: 4/3/2017 3:30:13 PM
 *  Author: marwa079
 */ 


#define 	F_CPU   14745600UL
#include "../global_definitions.h"
#include "../SensorModule.h"
#include "laser_mode.h"
#include "../UART.h"
#include "test_mode.h"
#include "../delay.h"
#include "drive_mode.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


volatile uint16_t tot_overflow;

#define vector_max_size 2000
volatile uint8_t magnet_count = 0; 
volatile uint16_t distance_vector[vector_max_size];
volatile uint16_t angle_vector[vector_max_size];
volatile uint16_t UART_data;
volatile uint16_t LIDAR_distance;

volatile uint16_t last_half_rotate_time = 0;
volatile uint16_t vector_count = 0;
volatile bool LSByte = false;


void Setup_timer0(void)
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

ISR(INT1_vect)
{
	last_half_rotate_time = tot_overflow; 
	++magnet_count;
	tot_overflow = 0;
}


ISR(USART0_RX_vect)
{
	
	
	
	if(get_LIDAR_16bit_data())
	{
		
		if(vector_count >= vector_max_size)
		{
			vector_count = 0;
		}
		else if(_steering_mode == 'L')
		{
		    distance_vector[vector_count] = UART_data;
		    angle_vector[vector_count] = Calculate_angle();
		    ++vector_count;	
		}
		else if (_steering_mode == 'D')
		{
			LIDAR_distance = UART_data;
			
		}
		
	}
	
	
	
}

//-----------------------------------------------------------------------------



uint16_t Calculate_angle(void)
{
	//muliply this with any number to get angle
	return (((double)tot_overflow / last_half_rotate_time) * 1000);
}



//activates or deactivates Overflow interrupts for the timer
void Activate_or_deactivate_counter0(bool activate_count)
{
	
	if(activate_count)
	{
		tot_overflow = 0;
	    TIMSK0 |= (1<<TOIE0);	
	    
	}
	else
	{
		TIMSK0 &= ~(1<<TOIE0);
		tot_overflow = 0;
		
	}
}





//reads the LIDAR vector_max_size times. Is used when LIDAR is rotating.
void Laser_speed_mode(void)
{
	cli();
	Activate_or_deactivate_counter0(true);
	magnet_count = 0;
	 
	sei();
	while(!Steady_LIDAR_ang_vel()) {};
	cli();
	
	Enable_USART_interrupt();
	USART_Transmit('L');
	
	sei();
	PORTD |= (1<<PORTD6);
	while(vector_count < vector_max_size);
	cli();
	
	USART_Transmit('D');
	Disable_USART_interrupt();
	Activate_or_deactivate_counter0(false);
	PORTD |= (1<<PORTD5);
	vector_count = 0;
	magnet_count = 0;
	
	sei();
}





bool Steady_LIDAR_ang_vel(void)
{
	
	return (magnet_count >= 4);
	
}


//sets MSByte false if UART receives a 0.
//When the next interrupt comes, the MSByte will be set to true.
bool get_LIDAR_16bit_data(void)
{
	uint16_t temp_data = UDR0;
	
	bool temp_bool = false;
	
	if(temp_data == 0xFF)
	{
		LSByte = true;
	}
	else if((temp_data != 0xFF) && LSByte)
	{
		UART_data = temp_data;
		LSByte = false;
	}
	else if(!LSByte)
	{
		temp_data = (temp_data << 8);
		UART_data |= temp_data;
		UART_data = (UART_data >> 1);
	    temp_bool = true;
	}
	
	return temp_bool;
	//maybe transmit further or do something with the data
	
}


//currently not used
uint16_t Single_reading_LIDAR(void)
{
	USART_Transmit('S');
	uint8_t low_byte = USART_Receive();
	uint16_t high_byte = USART_Receive();
	
	
	uint16_t return_val = (high_byte << 8) | low_byte;
	return return_val;
	
}



//returns false if something failed
void send_LIDAR_values(uint8_t delay_us)
{
	
	for(uint16_t i = 0 ; i < vector_max_size ; ++i)
	{  
		spi_send_to_module(0xFF, comm_ss_port_);
		delay(delay_us);
		spi_send_to_module(0xFF, comm_ss_port_);
		delay(delay_us);
		
		
		spi_send_to_module((distance_vector[i]>>8), comm_ss_port_);
		delay(delay_us);
		spi_send_to_module(distance_vector[i], comm_ss_port_);
		delay(delay_us);
		
		spi_send_to_module((angle_vector[i]>>8), comm_ss_port_);
		delay(delay_us);
		spi_send_to_module((angle_vector[i]), comm_ss_port_);
		delay(delay_us);
	    
	
	}
	

	spi_send_to_module(0x00, comm_ss_port_);
	delay(delay_us);
	spi_send_to_module(0x00, comm_ss_port_);
	delay(delay_us);

}



void Activate_or_deactivate_hall2(bool act_or_de)
{
	if(act_or_de)
	{
		EIMSK |= (1<<INT0);
	}
	else
	{
		EIMSK &= ~(1<<INT0);
	}
}





void LIDAR_mode(void)
{
	 PORTD |= (1<<PORTD7);
	 Laser_speed_mode();
	 
	 spi_send_to_module(0xFF, steering_ss_port_);
	 send_LIDAR_values(50);
	 //Activate_or_deactivate_hall2(true);
	 //while(!LIDAR_straight);
	 //Activate_or_deactivate_hall2(false);
	 //LIDAR_straight = false;
	 PORTD &= ~((1<<PORTD4) | (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7));
	 uint8_t curr_steering_mode = spi_send_to_module(0x00, comm_ss_port_);
	 
	 Check_mode_change(curr_steering_mode);
	 
}