/*
 * Main file for game on pic processor
 * 
 * Copyright (c) 2016   Hannes Rabo
 * Copyright (c) 2016   Julius Celik
 */ 
#include "main.h"

#include "spi.h"

int reset()
{
    int i;
    for(i = 0; i < max_number_of_players; i++)
        points[i] = 0;
    
    for(i = 0; i < question_buffer_size; i++)
        current_question[i] = 0;
    
    current_answer = -1;
	
	return 0;
}

void init()
{

	//TRISE &= ~0xff;
	//PORTE = 0;

    reset();
    
    spi_init();
    random_init();        
    buttons_init();
    enable_interrupt();
	
	//Game init needs interupts
    game_init();
}

int main() {
	init();
		
	while(1)
	{
		poll_button();
	}
        
	return 0;
}
