#ifndef GUARD_GAME
#define GUARD_GAME

// These functions are used to update the currently stored value
//////////////////////////////////
int update_number_of_questions(char number);	//Called from the spi handler
int update_answer(char answer);					//Update the current number for correct answer
int update_score(char player_id);

// Update stuff at the pi
/////////////////////////////////
int game_init(void);								//Initiate game
int game_end(void);									//end current game
int game_answer(char player_id, char button_id); 	//update the pi with the new answer

int send_number_of_players(void);					//Send the number of players to pi
int send_answer(char player_id, char correct);		// Send who answered, if the answer was correct, and the updated scores

//	Call these if you need the get the values. 
// These functions will stall the execution of the program
////////////////////////////////////////////////////////////
int query_number_of_questions(void);			//Call this method to update number of query_number_of_questions (this function will stall)
int query_next_answer(void);					//getting the next ans (this function will stall)

#endif //GUARD_GAME
