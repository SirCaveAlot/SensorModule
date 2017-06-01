/*
 * UART.h
 *
 * Created: 4/10/2017 1:20:44 PM
 * Author: Marcus Wallin, marwa079
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#define clkspd 14745600
#define BAUD 115200
#define UBBR clkspd/16/BAUD-1

void USART_Init( unsigned int baud );

void Enable_USART_interrupt(void);

void Disable_USART_interrupt(void);

void USART_Transmit(uint8_t data);

bool get_LIDAR_16bit_data(void);

uint8_t USART_Receive( void );

#endif /* UART_H_ */