/*
 * Spi slave for pic processor
 * Event based.
 *
 * Copyright Julius Celik 2016
 * 	     Hannes Rabo 2016
 */

#include "spi.h"



int spi_init(void)
{
	
	//Setup interrupts
	IFS(0) &= ~(1 << 7);
	IEC(0) |= (1 << 7);
	IPC(1) |= (7 << 26);
	IPC(1) |= (3 << 24);
	
	//MOSI/MISO
	TRISD &= ~0x1;
	TRISF |= 2;
	
	//Clock
	TRISD |= 1 << 8;
	
	//Callback (interrupt to pi)
	TRISD &= ~2;
	PORTD |= 2;
	
	SPI_BUFFER = 0xff;

	spi_reset(); 
	

	return 0;
}

void spi_request_attention()
{
	PORTD &= ~(2);
	
}

int spi_transmit_bit(void) 
{
	
	//READ
	if(cyclecount & 1){	
		// Startbit		
		if(cyclecount == 1) {
            
			//If the start-bit is 0, something is wrong. 
			//We need to resend this message
			if(!GET_IN || error) //Error on first bit or xor
			{
				cyclecount = 0;
				return -1;
				
			}
			
			//This will update the current buffer with relevant data
			//(old or new)
			SPI_BUFFER = DATA_BUFFER[DATA_BUFFER_COUNT];
		}
		// XOR_bit (this is last in the message)
		else if(cyclecount == 19) {

			if(GET_IN != xor_in)
			{	
                error = 1;
				
			} 
			else if(!error)
            {
                //Store the previous buffer
                DATA_BUFFER[DATA_BUFFER_COUNT] = SPI_BUFFER;

                //We need to update the buffer position
                DATA_BUFFER_COUNT++;
                
            }
            
            //Handle full message
            if(DATA_BUFFER_COUNT == STATIC_BUFF_SIZE )
            {   
                spi_error = spi_message_handler(DATA_BUFFER);
			
                int i;
                for(i = 0; i < STATIC_BUFF_SIZE; i++)
                {
                    DATA_BUFFER[i] = 0x0;
                }
                
                DATA_BUFFER_COUNT = 0;
                error = 0;
            }
            
            //Initiate globals
            cyclecount 	= 0;
            xor_in		= 0;
            xor_out		= 0;
			
			PORTD |= 2;
			
            return 0;
            
		}
		// Message-bits
		else {
			//Moving input to buffer and update xor checksum
			char input = GET_IN;
			SPI_BUFFER = (SPI_BUFFER & 0x7F) |  (input << 7);
			xor_in ^= input;
		}

		
	//WRITE
	}else {

		// Startbit		
		if(cyclecount == 0) {

			//0 means ok, 1 means resend
			if(error)
			{
				SET_OUT(0);
				error = 0;	
			} 
			else 
			{
				SET_OUT(1);
				
			}
		}
		// XOR_bit
		else if(cyclecount == 18) {
			SET_OUT(xor_out);
            xor_out = 0;

		}
		// Message-bits
		else {
			//Setting the output to first bit of buffer
			//Calculate xor checksum
			SET_OUT(SPI_BUFFER & 1);
			xor_out ^= SPI_BUFFER & 1;


			// Shifting buffer.
			SPI_BUFFER = (SPI_BUFFER >> 1);

		}

    }
	
	cyclecount++;
	
	
	return 0;
}

/*
 * All messages are exactly 8 bytes long
 * and only includes the actual data (the other is handled automatically.
 */
int spi_dump_message(const char* msg) {
	
	//Setting busy flag
	if(0 && spi_busy)
		return -1;
	
	spi_busy = 1;
	
	int i;
	for(i = 0; i < STATIC_BUFF_SIZE; i++)
	{
		DATA_BUFFER[i] = msg[i];
		LAST_BUFFER[i] = msg[i];
	}
	
	return 0;
	
}

int spi_reset(void)
{
	//Initiate globals
	cyclecount 	= 0;
	xor_in		= 0;
	xor_out		= 0;
	error 		= 0;

	spi_error   = 0;
	spi_busy    = 0;
        
	SPI_BUFFER = 0;

	int i;
	for(i = 0; i < STATIC_BUFF_SIZE; i++)
	{
		DATA_BUFFER[i] = 'a';
	}
    
	DATA_BUFFER_COUNT = 0;
	
	return 0;
}


