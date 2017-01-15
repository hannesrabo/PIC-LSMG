/*
 * Button handler for pic processor
 * Event based.
 *
 * Copyright Julius Celik 2016
 * Copyright Hannes Rabo  2016
 */

#include "button_handler.h"

#include "spi_handler.h"
#include "spi.h"

char button_pressed;

int buttons_init(void)
{
	PORTE = 0;
	TRISE = 0x00ff;
	// Setup button current (RD9 - RD10) (PIN 7-8)
// 	TRISD &= ~(3 << 9);
	
	// Setup button current (RF2 - RF3) (pin 33-34)
// 	TRISF &= ~(3 << 2);

	// Setup button input (RD5 - RD7) (PIN 34, 36-37)
// 	TRISD |= 7 << 5;

	// Setup for button input (RF4-RF6) (PIN 38 - 40)
// 	TRISF |= 7 << 4;
	
// 	PORTD = 0;
// 	PORTD |= (3 << (9));
	
	button_pressed = 0;
	return 0;
}

int poll_button()
{
	int i;
	if(!button_pressed){
		
		for(i = 0; i < 6; i++)
		{
			if((PORTE >> i) & 1)
			{
				button_pressed = 1;
				
				char player_id;
				char button_id;
				
				
				if(PORTE & 0x7){
					player_id = 1;
					button_id = i;
				}
				else {
					player_id = 0;
					button_id = i - 3;
				}
				
				//update_score(0);

				game_answer(player_id, (2 - button_id));

				
// 			    spi_request_number_of_questions();
// 				// 	int i;
// 				for(i = 0; i < 10000000; i++)
// 					;
// 				
// 				spi_request_attention();
				//query_number_of_questions();
			}
			
		}
		
	//IF no buttons are pressed
	} 
	else if( !(PORTE & 0x3f) )
	{
		button_pressed = 0;
		
	}
	
	return 0;
}
