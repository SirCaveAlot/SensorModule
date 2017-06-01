/*
 * spi.c
 *
 * Created: 4/10/2017 1:20:24 PM
 *  Author: Marcus Wallin, marwa079
 */ 


#include <inttypes.h>
#include <avr/io.h>
#include "spi.h"

#define __SPI_PORT PORTB
#define __SPI_DDR DDRB
const uint8_t __SPI_MOSI = 5;
const uint8_t __SPI_MISO = 6;
const uint8_t __SPI_SCK = 7;
const uint8_t __SPI_LSBFIRST_MASK = 0b00000001;
const uint8_t __SPI_MASTER_MASK = 0b00000001;
const uint8_t __SPI_MODE_MASK = 0b00000011;
const uint8_t __SPI_SPEED_MASK = 0b00000011;
const uint8_t __SPI_DBLCLK_MASK = 0b00000001;

//function found on the internet
//initialize the SPI bus
//  uint8_t lsbfirst - if 0: most significant bit is transmitted first
//  uint8_t master - if 1: use master mode, if 0: slave mode is used
//  uint8_t mode - sets the transfer mode:
//                 mode   leading clock edge   trailing clock edge
//                 -----------------------------------------------
//                 0      sample (rising)      setup  (falling)
//                 1      setup  (rising)      sample (falling)
//                 2      sample (falling)     setup  (rising)
//                 3      setup  (falling)     sample (rising)
//  uint8_t clkrate - spi bus clock rate, valid speeds are 0-3
//                    rate   speed
//                    ------------
//                    0      CPUCLK/4
//                    1      CPUCLK/16
//                    2      CPUCLK/64
//                    3      CPUCLK/128
//  uint8_t dblclk - if 1: doubles the SPI clock rate in master mode
//  EXAMPLE: spi_init(0, 1, 0, 3, 0)
void spi_init(uint8_t lsbfirst,
              uint8_t master,
              uint8_t mode,
              uint8_t clkrate,
              uint8_t dblclk){
  //set outputs
  __SPI_DDR |= ((1<<__SPI_MOSI) | (1<<__SPI_SCK));
  //set inputs
  __SPI_DDR &= ~(1<<__SPI_MISO);
  __SPI_PORT |= (1<<__SPI_MISO); //turn on pull-up resistor
  //set SPI control register
  SPCR = (
           (1<<SPE) | //enable SPI
           ((lsbfirst & __SPI_LSBFIRST_MASK)<<DORD) | //set msb/lsb ordering
           (master<<MSTR) | //set master/slave mode
           ((mode & __SPI_MODE_MASK)<<CPHA) | //set mode
           (clkrate & __SPI_SPEED_MASK<<SPR0) //set speed
         );
		 
  //SPCR |= (1<<MSTR);
  //set double speed bit
  SPSR = ((dblclk&__SPI_DBLCLK_MASK)<<SPI2X);
}

//function found on the internet
//shifts out 8 bits of data
//  uint8_t data - the data to be shifted out
//  returns uint8_t - the data received during sending
uint8_t spi_send(uint8_t value){
  uint8_t result;

  //shift the first byte of the value
  SPDR = value;
  //wait for the SPI bus to finish
  while(!(SPSR & (1<<SPIF)));
  //get the received data
  result = SPDR;

  return result;
}


//sets ss to low at specified module
void ss_to_low(uint8_t port_nr)
{
	PORTB &= ~port_nr;
}


//sets ss to high at specified module
void ss_to_high(uint8_t port_nr)
{
	PORTB |= port_nr;
}


//sends an 8 bit value to a specified module
char spi_send_to_module(uint8_t val, uint8_t module_choice)
{
	char return_mode;
	
	ss_to_low(module_choice);
	
	return_mode = spi_send(val);
	
	ss_to_high(module_choice);
	
	return return_mode;
}