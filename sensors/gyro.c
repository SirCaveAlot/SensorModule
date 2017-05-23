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
#include "../communication/spi.h"
#include "gyro.h"
#include "../global_definitions.h"



//Activates the gyro and checks if the instruction was sent properly
//if not this will return false.
bool Activate_gyro(void)
{

	uint8_t high_byte = 0;

	ss_to_low(gyro_ss_port_);
	spi_send(0b10010100);
	high_byte = spi_send(0);
	spi_send(0);
	ss_to_high(gyro_ss_port_);
	
	
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
	ss_to_low(gyro_ss_port_);
	spi_send(0b10001000);
	high_byte = spi_send(0);
    spi_send(0);
	ss_to_high(gyro_ss_port_);
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
	
	ss_to_low(gyro_ss_port_);
	spi_send(0b10010100);
	spi_send(0b00000000);
	ss_to_high(gyro_ss_port_);
	
	//check if retrieved high_bit
	
}

//Reads Gyro until data is receieved (EOC is high).
//Returns the AD converted value in 8 bits.
uint16_t Read_gyro(void)
{
	bool EOC_high = false;
	uint16_t data;
	do 
	{
		ss_to_low(gyro_ss_port_);
		spi_send(0b10000000);
		data = spi_send(0);
		data = data << 8;
		data |= spi_send(0);
		ss_to_high(gyro_ss_port_);
		
		EOC_high = (1<<13) == ((1 << 13) & data); 
		
	} while (!EOC_high);
	
	data = (data & 0x0FFE);
	
	//change to 8 bit
	return (data >> 1);
	
}


//Calls Starts conversion and reads value. This function should be called everytime 
//a reading shoud be done from the gyro.
uint16_t Get_angular_velocity(void)
{
	
	Start_conversion_gyro();

	return Read_gyro();
}
