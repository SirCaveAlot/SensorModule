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



//extern char UART_data;

uint8_t sensor_values[8];

volatile uint8_t curr_sensor;

bool _gyro_activated = false;

uint8_t count = 0;	

bool max_speed_bool = false;

//------------------SETUP----------------------------------------------
//EIMSK sets INT0 as interrupt
//EICRA activates interrupts on rising edge
//ADMUX sets the highest bits in the AD converter to ADCH
void interrupt_setup(void)
{
	
	EIMSK = (1<<INT0); //| (1<<INT2);
	EICRA = (1<<ISC01) | (1<<ISC00);
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
	_steering_mode = 'T';
	
	//DDRB |= (1<<DDB3) ;
	
}

volatile bool speed_bool = false;
//interrupt vector for the ADC

//sets up the SPI properly and activates the Gyro.
ISR(INT0_vect)
{
	speed_bool = true;
	
	
	
	   /* max_speed_bool = !max_speed_bool; 
	if(max_speed_bool)
	{
		Enable_USART_interrupt();
	}
	else
	{
		Disable_USART_interrupt();
	}
	*/
	
}



//--------------MAIN----------------------------------------------------------



int main(void)
{   
	//set up for global variables
	/*sensor_values_zero();
	ADread = 0;
	curr_sensor = 0;
	*/
	Overall_setup();
	interrupt_setup();
	spi_init(0,1,0,1,0);
	
	SPI_setup();
	USART_Init(UBBR);
	DDRD |= (1<<DDD6);

	uint8_t gyro_value = 0;
	uint16_t LIDAR_value = 0;
	//DDRB = 0xFF;
	
	Setup_timer();
	sei();
	
	while(1)
	{
		PORTD |= 1<<PORTD6;
		read_analog_sensors(0xFF);
		gyro_value = Get_angular_velocity();
		LIDAR_value = Single_reading_LIDAR();
		
		Send_all_values(gyro_value, LIDAR_value, comm_ss_port_);
		
		PORTD &= ~(1<<PORTD6);
		
		_delay_ms(1000);
		/*
		if(speed_bool)
		{
         Laser_speed_mode();
		 speed_bool = false;
		 //test_Laser_max_freq();
		}*/
		
		
	};
}



void Send_all_values( uint8_t gyro_val, uint16_t LIDAR_val , uint8_t module_choice)
{
	for(uint8_t i = 0 ; i < 8 ; ++i)
	{
	    test_spi_send(_analog_sensor_values[i], steering_ss_port_);
		test_spi_send(_analog_sensor_values[i], comm_ss_port_);
	    _delay_us(100);	
	}
	
	test_spi_send(gyro_val, steering_ss_port_);
	test_spi_send(gyro_val, comm_ss_port_);
	
	 _delay_us(100);
	 
	 
	uint8_t LIDAR_low_byte = (uint8_t) LIDAR_val;
	uint8_t LIDAR_high_byte = (uint8_t) (LIDAR_val>>8);
	test_spi_send(LIDAR_high_byte, steering_ss_port_);
	test_spi_send(LIDAR_high_byte, comm_ss_port_);
	_delay_us(100);
	test_spi_send(LIDAR_low_byte, steering_ss_port_);
	test_spi_send(LIDAR_low_byte, comm_ss_port_);
	
	_delay_us(100);
	test_spi_send(0xFF, steering_ss_port_);
    test_spi_send(0xFF, comm_ss_port_);
	
}



void Mode_loop()
{

	//char curr_steering_mode = 'T';
    _steering_mode = 'T';

	while(1)
	{
                              //change this function
		if(  manual_mode_ == (PORTD & 0b00001101))
		{
			
			
		}
		else
		{
			/*
			if(_steering_mode != curr_steering_mode)
			{
				_steering_mode = curr_steering_mode;
				//  Reset_globals();
				
			}
			*/
			switch(_steering_mode)
			{
				
				//drive straight mode
				case 'D':
				//  Straight_mode();
				break;
				
				
				//rotate mode
				case 'R':
				//Rotate_mode();
				break;
				
				
				//laser mode
				case 'L':
				
				break;
				
				//test mode
				case 'T':
				 //  Test_mode();
				break;
				
				default:
				
				//send 0 with delay
				break;
				
				
			}
			
		}
		
		
		
	}

}














double IR_conversion(uint8_t val)
{
	
	return (55.25 * exp(-0.05762 * val)) + (14.2 * exp(-0.009759 * val));
	
	
}




