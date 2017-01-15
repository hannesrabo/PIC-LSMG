#pragma once

#ifndef GUARD_spi
#define GUARD_spi


#include <pic32mx.h>
#include "spi_handler.h"

#define SET_OUT(x)	PORTD = (PORTD & ~1) | (x)
#define GET_IN		(PORTF & 2) >> 1
#define GET_SCLK	(PORTD >> 8) & 1

#define STATIC_BUFF_SIZE	8

unsigned char LAST_BUFFER[STATIC_BUFF_SIZE];
unsigned char DATA_BUFFER[STATIC_BUFF_SIZE];
unsigned char SPI_BUFFER;

char spi_error;
char spi_busy;

unsigned char DATA_BUFFER_COUNT;

char cyclecount;
char xor_in;
char xor_out;
char error;

int spi_init(void);
int spi_transmit_bit(void);
int spi_dump_message(const char* msg);
int spi_reset(void);

void spi_request_attention();

#endif //GUARD_spi







