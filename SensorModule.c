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
	DDRD |=  (1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7);

	//uint8_t gyro_value = 0;
	//uint16_t LIDAR_value = 0;
	//DDRB = 0xFF;
	
	Setup_timer0();
	Setup_timer2();
	sei();
	
	Mode_loop();
	/*
	while(1)
	{
		PORTD |= 1<<PORTD6;
		
		
		if(speed_bool)
		{
         Laser_speed_mode();
		 speed_bool = false;
		 //test_Laser_max_freq();
		}
		
		
	};
	*/
}








void Mode_loop(void)
{

	char curr_steering_mode = _steering_mode;
    

	while(1)
	{
                              //change this function
		//if(  manual_mode_ == (PORTD & 0b00001101))
		//{
			
			
		//}
		//else
		//{
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
				    Activate_or_deactivate_counter2(true);
					
					read_analog_sensors(0x3F);
					uint8_t gyro_value = Get_angular_velocity();
					PORTD |= (1<<PORTD6);
					uint16_t LIDAR_value = Single_reading_LIDAR();
				    
					
					if(Send_all_values(gyro_value, LIDAR_value, comm_ss_port_))
					{
						Activate_or_deactivate_counter2(false);
						break;
					}
					
					
					//send with frequency of 50 hz
					while(tot_overflow_send < 18);
				    Activate_or_deactivate_counter2(false);
					
				break;
				
				
				//rotate mode
				case 'L':
				    PORTD |= (1<<PORTD7);
					Laser_speed_mode();
					
					test_spi_send(0xFF, steering_ss_port_);
					send_LIDAR_values(80);
	//				Activate_or_deactivate_hall2(true);
	//				while(!LIDAR_straight);
	//				Activate_or_deactivate_hall2(false);
					LIDAR_straight = false;
					PORTD &= ~((1<<PORTD4) | (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7));
					curr_steering_mode = test_spi_send(0x00, comm_ss_port_);
					Check_mode_change(curr_steering_mode);
					//add some function to send all values
				break;
				
				
				//laser mode
				case 'S':
				     
					 
				    
				     curr_steering_mode = test_spi_send(0x00, comm_ss_port_);
					 Check_mode_change(curr_steering_mode);
					 test_spi_send(0xFF, steering_ss_port_);
				     PORTD |= (1<<PORTD5);
					 
						 
					 _delay_ms(100);
				break;
				
				//test mode
				case 'T': 
				   
					 Send_0_to_9();
					
					
				break;
				
				default:
				
				
				    PORTD &= ~((1<<PORTD4) | (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7));
				    curr_steering_mode = test_spi_send(0x00, comm_ss_port_);
					Check_mode_change(curr_steering_mode);
				
				break;
				
				
			}
			
		//}
		
		
		
	}

}





bool Check_mode_change(char curr_steering_mode)
{
	bool mode_changed = (curr_steering_mode != _steering_mode);
	
	 _steering_mode = curr_steering_mode;
	
	if(mode_changed)
	{
		PORTD &= ~((1<<PORTD4) | (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7));
	}
	
	return mode_changed;
	
}



bool Send_all_values( uint8_t gyro_val, uint16_t LIDAR_val , uint8_t module_choice)
{
	

    if(Send_value_both_modules(0xFF))
    {
	    return true;
    }

    
	if(Send_value_both_modules(0xFF))
	{
		return true;
	}
	
	//------SEND ANALOGS---------------------------------
	
	for(uint8_t i = 0 ; i < 6 ; ++i)
	{
		
		if(Send_value_both_modules(_analog_sensor_values[i]))
		{
			return true;
		}
		
	}
	//------SEND GYRO---------------------------------
	
	if(Send_value_both_modules(gyro_val))
	{
		return true;
	}
	
	//------SEND LIDAR---------------------------------
	uint8_t LIDAR_low_byte = (uint8_t) LIDAR_val;
	uint8_t LIDAR_high_byte = (uint8_t) (LIDAR_val>>8);
	if(Send_value_both_modules(LIDAR_high_byte))
	{
		return true;
	}
	
	
	if(Send_value_both_modules(LIDAR_low_byte))
	{
		return true;
	}
	
	//------SEND STOP BYTE---------------------------------
	/*if(Send_value_both_modules(0xFF))
	{
		return true;
	}*/
	
	return false;
}




bool Send_value_both_modules(uint8_t send_value)
{
	char curr_steering_mode = test_spi_send(send_value, comm_ss_port_);
	if(Check_mode_change(curr_steering_mode))
	{
		_delay_us(80);
		return true;
	}
	
	test_spi_send(send_value, steering_ss_port_);
	_delay_us(80);
	
	return false;
}



double IR_conversion(uint8_t val)
{
	
	return (55.25 * exp(-0.05762 * val)) + (14.2 * exp(-0.009759 * val));
	
	
}




