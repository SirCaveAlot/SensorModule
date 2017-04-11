/*
 * UART.c
 *
 * Created: 4/10/2017 1:20:24 PM
 *  Author: marwa079
 */ 
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "UART.h"

uint16_t UART_data;

bool MSByte;

void USART_Init( unsigned int baud )
{
	/* Set baud rate */
	UBRR0H = 0;
	UBRR0L = 7;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = 0b00000110;
}



void Enable_USART_interrupt(void)
{
	UCSR0B |= (1<<RXCIE0);
	sei();
}


void Disable_USART_interrupt(void)
{
	UCSR0B &= ~(1<<RXCIE0);
}


void USART_Transmit(uint8_t transmit_data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = transmit_data;
}



//sets MSByte false if UART receives a 0. 
//When the next interrupt comes, the MSByte will be set to true.

ISR(USART0_RX_vect)
{
	get_LIDAR_16bit_data();
	
}



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
	
}




