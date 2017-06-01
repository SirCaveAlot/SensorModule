/*
 * SensorModule.c
 *
 * Created: 3/28/2017 9:41:33 AM
 *  Author: Marcus Wallin, marwa079
 */ 


#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include <math.h>
#include <stdbool.h>

#include "SensorModule.h"
#include "communication/spi.h"
#include "sensors/gyro.h"
#include "sensors/analog_sensors.h"
#include "global_definitions.h"
#include "communication/UART.h"
#include "modes/laser_mode.h"
#include "modes/drive_mode.h"
#include "delay.h"



uint8_t sensor_values[8];

volatile uint8_t _curr_sensor;

volatile char _steering_mode = 'D';


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
	
	DDRB |= (1<<DDB0) | (1<<DDB1) | (1<<DDB4);

	PORTB |= gyro_ss_port_ | comm_ss_port_ | steering_ss_port_;
	Start_gyro();
}



void Overall_setup(void)
{
	
	_comm_mode = 'T';
	_steering_mode = 'D';

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







//the big function where the loop takes place.
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
				     curr_steering_mode = spi_send_to_module(0x00, comm_ss_port_);
					 Check_mode_change(curr_steering_mode);
					 spi_send_to_module(0x00, steering_ss_port_);
				     PORTD |= (1<<PORTD5);
					 
				break;
				
				//test mode
				case 'T': 
				     delay_100_ms();
					 PORTD |= (1<<PORTD7);
					 curr_steering_mode = spi_send_to_module(0x00, comm_ss_port_);
					 Check_mode_change(curr_steering_mode);
					 
				break;
				
				default:
				
				    delay_100_ms();
				    PORTD &= ~((1<<PORTD4) | (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7));
				    curr_steering_mode = spi_send_to_module(0x00, comm_ss_port_);
					Check_mode_change(curr_steering_mode);
				    
				break;
				
				
			}
	}

}


