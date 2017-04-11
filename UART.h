/*
 * UART.h
 *
 * Created: 4/10/2017 1:20:44 PM
 *  Author: marwa079
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

void get_LIDAR_16bit_data(void);

extern uint16_t UART_data;

#endif /* UART_H_ */