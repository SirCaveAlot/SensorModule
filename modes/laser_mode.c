/*
 * laser_mode.c
 *
 * Created: 4/3/2017 3:30:13 PM
 *  Author: Marcus Wallin, marwa079
 */ 


#define 	F_CPU   14745600UL
#include "../global_definitions.h"
#include "../SensorModule.h"
#include "laser_mode.h"
#include "../communication/UART.h"
#include "test_mode.h"
#include "../delay.h"
#include "drive_mode.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>


volatile uint16_t tot_overflow;

#define vector_max_size 2000
volatile uint8_t magnet_count = 0; 
volatile uint16_t distance_vector[vector_max_size];
volatile uint16_t angle_vector[vector_max_size];
volatile uint16_t distance_data;
volatile uint16_t angle_data;



volatile uint16_t LIDAR_distance;

volatile uint16_t last_full_rotate_time = 0;
volatile uint16_t vector_count = 0;



void Setup_timer0(void)
{
	//prescaler on /8
	TCCR0B = (1<<CS01) ;
	TCNT0 = 0;
	
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
//can use the same variable for both magnet count and LIDAR_half
ISR(INT1_vect)
{
	last_full_rotate_time = tot_overflow; 
	
	++magnet_count;
	
	tot_overflow = 0;
}

//interrupt vector for UART0
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
			
		    distance_vector[vector_count] = distance_data;
		    angle_vector[vector_count] = angle_data;//Calculate_angle();
		   
			++vector_count;	
		}
		else if (_steering_mode == 'D')
		{
			LIDAR_distance = distance_data;
			
		}
	}
}


//-----------------------------------------------------------------------------

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
	
	magnet_count = 0;
	vector_count = 0;
	sei();

	Enable_USART_interrupt();
	USART_Transmit('L');

	PORTD |= (1<<PORTD6);
	
	while(vector_count < vector_max_size);

	cli();
	
	USART_Transmit('D');
	Disable_USART_interrupt();

	PORTD |= (1<<PORTD5);
	vector_count = 0;
	magnet_count = 0;
	
	sei();
}




volatile uint8_t data_counter;
//sets MSByte false if UART receives a 0.
//waits for the LIDAR value to be complete.
//returns true if all data is received in the current mode
//if false all data is not received
bool get_LIDAR_16bit_data(void)
{
	uint16_t temp_data = UDR0;
	
	bool temp_bool = false;
	
	if(temp_data == 0xFF && data_counter == 0)
	{
		data_counter = 1;
		
	}
	else if(data_counter == 1)
	{
		
		data_counter = 2;
		distance_data = (temp_data<<8);
		
	}
	else if(data_counter == 2)
	{
		if(_steering_mode == 'L')
		{
		   data_counter = 3;	
		}
		else
		{
           data_counter = 0;
		   temp_bool = true;
		}
		
		distance_data |= temp_data;
		distance_data = (distance_data >> 1);
	    
	}
	else if(data_counter == 3)
	{
		
		angle_data = (temp_data << 8);
		data_counter = 4;
		
	}
	else if(data_counter == 4)
	{   
		
		angle_data |= temp_data;
		data_counter = 0;
		temp_bool = true;
	}
	return temp_bool;

	
}


//currently not used
//sends an instruction to the LIDAR tower to starta measurement and send it back.
uint16_t Single_reading_LIDAR(void)
{
	USART_Transmit('S');
	uint8_t low_byte = USART_Receive();
	uint16_t high_byte = USART_Receive();
	
	
	uint16_t return_val = (high_byte << 8) | low_byte;
	return return_val;
	
}



//sends all LIDAR values to the communication module
void send_LIDAR_values(uint8_t delay_us)
{
	
	for(uint16_t i = 0 ; i < vector_max_size ; ++i)
	{  
		spi_send_to_module(0xFF, comm_ss_port_);
		_delay_us(250);
		spi_send_to_module(0xFF, comm_ss_port_);
		_delay_us(250);
		
		
		spi_send_to_module((distance_vector[i]>>8), comm_ss_port_);
		_delay_us(250);
		spi_send_to_module(distance_vector[i], comm_ss_port_);
		_delay_us(250);
		
		spi_send_to_module((angle_vector[i]>>8), comm_ss_port_);
		_delay_us(250);
		spi_send_to_module((angle_vector[i]), comm_ss_port_);
		_delay_us(250);
	    
	
	}

}



//main function used in LIDAR mode ('L')
void LIDAR_mode(void)
{
	 PORTD |= (1<<PORTD7);
	 Laser_speed_mode();
	 
	
	 send_LIDAR_values(100);

	 PORTD &= ~((1<<PORTD4) | (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7));
	
	 uint8_t curr_steering_mode;
	 do 
	 {
		 curr_steering_mode = spi_send_to_module(0x00, comm_ss_port_);
		 delay(50);
	 } while (curr_steering_mode == 'L');
	 
	 
	 Check_mode_change(curr_steering_mode);
	 
}