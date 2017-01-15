/*
 * Main event handler. This forwards the event to relevant code
 * 
 * Copyright (c) 2016 Hannes Rabo
 * Copyright (c) 2016 Julius Celik
 */

#include "isr.h"

void user_isr( void )
{
	// SPI SCLK
	if(IFS(0) & (1 << 7))
	{
		spi_transmit_bit();
		IFS(0) &= ~(1 << 7);
	}
}


