/*
 * Copyright (c) 2016 Hannes Rabo
 */


#ifndef GUARD_game
#define GUARD_game

#include "random.h"
#include "globals.h"
#include "spi.h"
#include "spi_handler.h"

#define PLAYER_COUNT	2

// These functions are used to update the currently stored value
//////////////////////////////////
int update_number_of_questions(char number);	//Called from the spi handler
int update_answer(char answer);					//Update the current number for correct answer
int update_score(char player_id);

void update_score_to_add(int score);

// Update stuff at the pi
/////////////////////////////////
int game_init(void);								//Initiate game
int game_end(void);									//end current game
int game_answer(char player_id, char button_id); 	//update the pi with the new answer

int send_number_of_players(void);					//Send the number of players to pi
int send_answer(char player_id, char correct, char button_id);		// Send who answered, if the answer was correct, and the updated scores

//	Call these if you need the get the values. 
// These functions will stall the execution of the program
////////////////////////////////////////////////////////////
int query_number_of_questions(void);			//Call this method to update number of query_number_of_questions (this function will stall)
int query_next_answer(void);					//getting the next ans (this function will stall)

char get_number_of_questions(void);
#endif	//GUARD_game
