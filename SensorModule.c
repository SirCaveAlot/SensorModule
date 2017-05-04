/*
 * SensorModule.c
 *
 * Created: 3/28/2017 9:41:33 AM
 *  Author: marwa079
 */ 


#define 	F_CPU   14745600UL

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include <math.h>
#include <stdbool.h>

#include "SensorModule.h"
#include "spi.h"
#include "sensors/gyro.h"
#include "sensors/analog_sensors.h"
#include "modes/test_mode.h"
#include "global_definitions.h"
#include "UART.h"
#include "modes/laser_mode.h"
#include "modes/drive_mode.h"
#include "delay.h"



//extern char UART_data;

uint8_t sensor_values[8];

volatile uint8_t _curr_sensor;

volatile bool LIDAR_straight = false;
uint8_t count = 0;	

bool max_speed_bool = false;

volatile char _steering_mode = 'D';
//char _steering_mode = 'D';

//------------------SETUP----------------------------------------------
//EIMSK sets INT0 as interrupt
//EICRA activates interrupts on rising edge
//ADMUX sets the highest bits in the AD converter to ADCH
void interrupt_setup(void)
{
	
	EIMSK = (1<<INT1);
	EICRA = (1<<ISC01) | (1<<ISC00) | (1<<ISC11);
	ADMUX = (1<<ADLAR);
	
}

void SPI_setup(void)
{
	
	//ss signal to gyro, comm and steering
	DDRB |= (1<<DDB0) | (1<<DDB1) | (1<<DDB4);

	//add more values later

	PORTB |= gyro_ss_port_ | comm_ss_port_ | steering_ss_port_;
	Start_gyro();
}



void Overall_setup(void)
{
	
	_comm_mode = 'T';
	_steering_mode = 'D';
	
	//DDRB |= (1<<DDB3) ;
	
}

volatile bool speed_bool = false;
//interrupt vector for the ADC

//sets up the SPI properly and activates the Gyro.
ISR(INT0_vect)
{
	
	LIDAR_straight = true;
	
	
	
	
}



//--------------MAIN----------------------------------------------------------



int main(void)
{   
	
	Overall_setup();
	interrupt_setup();
	spi_init(0,1,0,1,0);
	
	SPI_setup();
	USART_Init(UBBR);
	DDRD |=  (1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7);


	
	Setup_timer0();
	Setup_timer2();
	sei();
	
	Mode_loop();

}








void Mode_loop(void)
{

	char curr_steering_mode = _steering_mode;
    
	while(1)
	{
			switch(_steering_mode)
			{
				
				//drive straight mode
				case 'D':
				    Drive_mode();
					
				break;
				
				//rotate mode
				case 'L':
				    LIDAR_mode();
				break;
				
				
				//stop mode
				case 'S':
				     
					 delay_100_ms();
				     curr_steering_mode = test_spi_send(0x00, comm_ss_port_);
					 Check_mode_change(curr_steering_mode);
					 test_spi_send(0x00, steering_ss_port_);
				     PORTD |= (1<<PORTD5);
					 
					 
				break;
				
				//test mode
				case 'T': 
				     delay_100_ms();
					 PORTD |= (1<<PORTD7);
					 curr_steering_mode = test_spi_send(0x00, comm_ss_port_);
					 Check_mode_change(curr_steering_mode);
					 
					
				break;
				
				default:
				
				    delay_100_ms();
				    PORTD &= ~((1<<PORTD4) | (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7));
				    curr_steering_mode = test_spi_send(0x00, comm_ss_port_);
					Check_mode_change(curr_steering_mode);
				    
				break;
				
				
			}
		
	}

}








bool Send_value_both_modules(uint8_t send_value)
{
	
	char curr_steering_mode = test_spi_send(send_value, comm_ss_port_);
	if(Check_mode_change(curr_steering_mode))
	{
		delay(80);
		return true;
	}
	
	
	test_spi_send(send_value, steering_ss_port_);
	delay(80);
	
	return false;
}




