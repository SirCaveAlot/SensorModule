/*
 * spi.h
 *
 * Created: 4/10/2017 1:20:24 PM
 *  Author: Marcus Wallin, marwa079
 */ 


#ifndef __SPI_H
#define __SPI_H

#include <inttypes.h>


void spi_init(uint8_t lsbfirst,
              uint8_t master,
              uint8_t mode,
              uint8_t clkrate,
              uint8_t dblclk);

uint8_t spi_send(uint8_t value);

void ss_to_low(uint8_t port_nr);

void ss_to_high(uint8_t port_nr);

char spi_send_to_module(uint8_t val, uint8_t module_choice);

#endif
