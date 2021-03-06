﻿/*
 * UART.c
 *
 * Created: 4/10/2017 1:20:24 PM
 *  Author: Marcus Wallin, marwa079
 */ 
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "UART.h"

//initiates USART
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

//sends a value via UART0 port
void USART_Transmit(uint8_t transmit_data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = transmit_data;
}

//waits until a value is received via UART0-port
uint8_t USART_Receive( void )
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}




