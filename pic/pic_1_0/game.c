/*
 * Main game logic for pic processor
 * Event based.
 *
 * Copyright Julius Celik 2016
 * Copyright Hannes Rabo 2016
 */

#include "game.h"

/////////////////////////////////////
#define NR_OF_QUESTIONS 7

char number_of_players = 2;
char correct_answer;

volatile char pi_answered = 0;
char settings_selected = 1;
char question_settings = 0;
char active_game = 0;
int player_score[4];
int number_of_questions;
int questions_asked_counter = 0;
int score_to_add = 0;
float punishment_factor = 0.7;

int game_init(void)
{
	active_game = 1;
	questions_asked_counter = 0;
	
	//Send number of players
 	spi_request_initiation(number_of_players);
	
	int i;	
	for(i = 0; i < number_of_players; i++)
	{
		player_score[i] = 0;
	}
	
	settings_selected = 0;
	
	spi_request_show_questions_alternatives();
	
	//We need to poll the buttons and wait for the list to be selected
	while(!settings_selected)
		poll_button();
	
	//send the answer
	spi_send_question_list(question_settings);
	
	//whait a little while
	for(i = 0; i < 50000000; i++)
		;
	
	query_number_of_questions();

	for(i = 0; i < number_of_questions; i++)
	{
		questions_asked[i] = 0;
	}

	// Get next answer from pi
	query_next_answer();
	
	return 0;
	
}

void update_score_to_add(int score)
{
	pi_answered = 1;
	score_to_add = score;
	
}

// Called by the pi
int update_number_of_questions(char number)
{
	number_of_questions = number;
	pi_answered = 1;
	
	return 0;
}

char get_number_of_questions(void)
{
	
	return number_of_questions;
}

// Ask pi to update number of questions on pic. The pi will call update_number_of_questions
int query_number_of_questions(void)
{
	
	// Wait for pi
	pi_answered = 0;
	
	//Request from pi
	spi_request_number_of_questions();

	// Wait for pi to answer
	while(!pi_answered);

	return 0;
}

// Called by the pi
int update_answer(char answer)
{
	correct_answer = answer;
	pi_answered = 1;
	
	return 0;
}

// Ask pi for the correct answer to next question. The pi will call update_answer
int query_next_answer(void)
{
	
	// Wait for pi
	pi_answered = 0;
	

	int i = 0;
	char question_id;
	
	i = get_random_int(number_of_questions);
	while(questions_asked[i])
	{
		i = get_random_int(number_of_questions);
	}
	
	question_id = i;
	questions_asked[i] = 1;

	
	// send question_id
	spi_request_next_answer(question_id);

	// Wait for pi to answer
	while(!pi_answered);
	
	questions_asked_counter++;

	return 0;
}


int send_answer(char player_id, char correct, char button_id)
{
	spi_send_player_answer(player_id, correct, button_id);
	
	return 0;
}

// Can expand to more complicated algorithms
int update_score(char player_id)
{
	spi_send_score(player_score[0], player_score[1]);
	return 0;
}

int game_end()
{
	spi_request_finish();
	return 0;
}

// Called when button is pressed
int game_answer(char player_id, char button_id)
{
	if(active_game && settings_selected)
	{
		//wait for the pi to answer
		pi_answered = 0;
		
		char correct = button_id == correct_answer;
		send_answer(player_id, correct, button_id);	// Send answer to pi	

		while(!pi_answered)
			;
		
		// Check for correct answer
		if(correct)
		{
			player_score[(int)player_id] += score_to_add;
		}
		else
		{
			// Give other players points
			if(player_id)
			{
				player_score[0] += score_to_add * punishment_factor;
			}
			else
			{
				player_score[1] += score_to_add * punishment_factor;
			}
		}

		
		update_score(player_id);
		
		
		//If the game is to end
		if(questions_asked_counter > NR_OF_QUESTIONS - 1 )
		{
			
			//No longer an active game
			active_game = 0;
			
			//Find out who won
			int player_index = 0;
			int i;
			
			for(i = 0; i < number_of_players; i++)
			{
				if(player_score[i] > player_score[player_index])
					player_index = i;
				
			}
			
			spi_send_player_won(player_index, player_score[player_index]);
			
			//We need to wait here fore a while so that the players have time to see who won
			for(i = 0; i < 100000000; i++)
				;
			
		}
		//Else just give us the next answer
		else
		{
			query_next_answer();				// Get next answer from pi
		}
	}
	else
	{
		if(settings_selected)
			//Start a new game
			game_init();
			
		else
		{
			//The user just pressed a button while in !settings_selected mode
			//This means that we should update the question list.
			settings_selected = 1;
			question_settings = button_id;
			
		}
			
	}
	
	return 0;
}
