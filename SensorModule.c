/*
 * SensorModule.c
 *
 * Created: 3/28/2017 9:41:33 AM
 *  Author: marwa079
 */ 


#define 	F_CPU   128000UL

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


//maybe change
//manual mode == high input on PD2


#define manual_mode_ 1<<2;
char _steering_mode;


uint8_t sensor_values[8];

volatile uint8_t curr_sensor;

bool _gyro_activated = false;

uint8_t value = 0;	

int main(void)
{   
	//set up for global variables
	/*sensor_values_zero();
	ADread = 0;
	curr_sensor = 0;
	*/
	
	interrupt_setup();
	spi_init(0,1,0,1,0);
	
	SPI_setup();
	DDRD |= (1<<DDD3);

	
	//DDRB = 0xFF;
	
	sei();
	
    while(1);
}





ISR(INT0_vect)
{
	
	//read_single_analog(2);
	//read_analog_sensors(nr);
	
	test_send_to_comm(0b00011000);
	
		
 //	value = Get_angular_velocity();
//		PORTD |= (1<<PORTD3);

/*
		Start_gyro();
		_gyro_activated = true;
		PORTD |= (1<<PORTD3);
	*/    
		
	
	
	
	
}


//interrupt vector for the ADC

//sets up the SPI properly and activates the Gyro.
void SPI_setup(void)
{
	
	//ss signal to gyro and comm
	DDRB |= (1<<DDB0) | (1<<DDB1) | (1<<DDB4);

	//add more values later

	PORTB |= (1<<PORTB0) | (1<<PORTB1) | (1<<PORTB4);
//	Start_gyro();
}



void Overall_setup(void)
{
	
	_comm_mode = 'T';
	_steering_mode = 'T';
	
}





void Mode_loop()
{

    char curr_steering_mode = T;


    while(1)
    {

      if(  manual_mode_ == (PORTD & manual_mode))
      {
          
    
      }
      else
      {
      
          if(_steering_mode != curr_steering_mode)
          {
              _steering_mode = curr_steering_mode;
            //  Reset_globals();
        
          }
    
          switch(_steering_mode)
          {
            
            //drive straight mode
            case D:
              //  Straight_mode();
                break;
            
            
            //rotate mode
            case R:
                //Rotate_mode();
                break;
              
              
            //laser mode
            case L:
        
                break;
        
            //test mode
            case T:
               // Test_mode();
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










void display_values(uint8_t nr)
{
	for(int i = 0 ; i < nr ; ++i)
	{
		
		PORTB = sensor_values[i];
		
		_delay_ms (1000);
		//delay_fcn(30000);
		
	}
	
	
}




//EIMSK sets INT0 as interrupt
//EICRA activates interrupts on rising edge
//ADMUX sets the highest bits in the AD converter to ADCH
void interrupt_setup(void)
{
	
	EIMSK = 1<<INT0;
	EICRA = (1<<ISC01) | (1<<ISC00);
	ADMUX = (1<<ADLAR);
	
}


