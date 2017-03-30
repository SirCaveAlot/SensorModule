/*
 * gyro.c
 *
 * Created: 3/29/2017 1:29:11 PM
 *  Author: marwa079
 */ 


//Code for the Gyro-sensor


#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include <stdbool.h>
#include "spi.h"

const uint8_t SPI_gyro = 0;

#define 



//ss to pb0, set to low
void ss_to_low_gyro(void)
{
	PORTB &= ~(1<<PORTB0);
}PORTB = (0<<PORTB0);

//set ss to high
void ss_to_high_gyro(void)
{
	PORTB |= (1<<PORTB0);
}



//Activates the gyro and checks if the instruction was sent properly
//if not this will return false.
bool Activate_gyro(void)
{
	//bool AD_ready = false;
	uint8_t high_byte = 0;
	//uint8_t low_byte = 0;
	
	ss_to_low_gyro();
	spi_send(0b10010100);
	high_byte = spi_send(0);
	spi_send(0);
	ss_to_high_gyro();
	
//	uint8_t result = high_byte >> 7;
	
	uint8_t shifted = high_byte >> 7;
	bool result = (shifted == 0);
	
	return result;
}


//sends read Status of the Gyro.
//returns true if EOC is set high.
//Thus means gyro is ready for new conversion.
bool Check_EOC_bit(void)
{
	uint8_t high_byte;
	ss_to_low_gyro();
	spi_send(0b10001000);
	high_byte = spi_send(0);
    spi_send(0);
	ss_to_high_gyro();
	high_byte = high_byte & (1<<5);
	return (1 == (high_byte >> 5));
}


//Calls Activate and then checks EOC bit until set.
//This function should be called in the initializing of the processor.

bool Start_gyro(void)
{
	bool gyro_ready = false;
	
	
	if(Activate_gyro())
	{
		do
		{
			gyro_ready = Check_EOC_bit();
		} while (!gyro_ready);
		
	}
	
	
	return gyro_ready;
}


//Starts a conversion in the gyro
void Start_conversion_gyro(void)
{
	
	ss_to_low_gyro();
	spi_send(0b10010100);
	spi_send(0);
	ss_to_high_gyro();
	
	//check if retrieved high_bit
	
}

//Reads Gyro until data is receieved (EOC is high).
//Returns the AD converted value in 8 bits.
uint8_t Read_gyro(void)
{
	bool EOC_high = false;
	uint16_t data;
	do 
	{
		ss_to_low_gyro();
		spi_send(0b10000000);
		data = spi_send(0);
		data = data << 8;
		data |= spi_send(0);
		ss_to_high_gyro();
		
		EOC_high = (1<<13) == ((1 << 13) & data); 
		
	} while (!EOC_high);
	
	data = (data & 0x0FFE);
	
	//change to 8 bit
	return (uint8_t)(data >> 4);
	
}


//Calls Starts conversion and reads value. This function should be called everytime 
//a reading shoud be done from the gyro.
uint8_t Get_angular_velocity(void)
{
	
	Start_conversion_gyro();

	return Read_gyro();
}
