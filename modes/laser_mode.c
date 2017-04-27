/*
 * laser_mode.c
 *
 * Created: 4/3/2017 3:30:13 PM
 *  Author: marwa079
 */ 

#include "../SensorModule.h"
#include "laser_mode.h"
#include "../UART.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint16_t tot_overflow;

#define vector_max_size 1000
volatile uint8_t magnet_count = 0; 
volatile uint16_t distance_vector[vector_max_size];
volatile uint16_t angle_vector[vector_max_size];
volatile uint16_t UART_data;

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






void Laser_speed_mode(void)
{
	cli();
	Activate_or_deactivate_counter0(true);
	
	PORTD |= (1<<PORTD6); 
	sei();
	while(!Steady_LIDAR_ang_vel()) {};
	cli();
	
	Enable_USART_interrupt();
	USART_Transmit('L');
	
	sei();
	PORTD |= (1<<PORTD5);
	while(vector_count < vector_max_size);
	cli();
	
	USART_Transmit('D');
	Disable_USART_interrupt();
	Activate_or_deactivate_counter0(false);
	PORTD |= (1<<PORTD4);
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



uint16_t Single_reading_LIDAR(void)
{
	USART_Transmit('S');
	uint8_t low_byte = USART_Receive();
	uint16_t high_byte = USART_Receive();
	
	
	uint16_t return_val = (high_byte << 8) | low_byte;
	return return_val;
	
}