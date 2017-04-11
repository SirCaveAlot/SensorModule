/*
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
bool magnet_detected;
volatile uint8_t magnet_count = 0; 




volatile uint16_t UART_data;

bool MSByte;


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
	++magnet_count;
}


ISR(USART0_RX_vect)
{
	get_LIDAR_16bit_data();
	
}

//-----------------------------------------------------------------------------


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
    
	while(Steady_LIDAR_ang_vel()) {};
	
	Enable_USART_interrupt();
	//change depending on letter
	USART_Transmit('T');
	
    
	
	
}





bool Steady_LIDAR_ang_vel(void)
{
	
	return (magnet_count >= 3);
	
}


//sets MSByte false if UART receives a 0.
//When the next interrupt comes, the MSByte will be set to true.


void get_LIDAR_16bit_data(void)
{
	uint16_t temp_data = UDR0;
	
	
	if(temp_data == 0xFF)
	{
		MSByte = false;
	}
	else if(!MSByte)
	{
		UART_data = temp_data;
		MSByte = true;
	}
	else if(MSByte)
	{
		temp_data = (temp_data << 8);
		UART_data |= temp_data;
		UART_data = (UART_data >> 1);
	}
	
	
	//maybe transmit further or do something with the data
	
}

