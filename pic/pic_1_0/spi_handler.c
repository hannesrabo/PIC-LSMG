#include "spi_handler.h"

/*
 * Handle incoming spi messages
 */
int spi_message_handler(const unsigned char* buffer)
{
    char code = *buffer;
    buffer++;
    
    int xor_error = spi_error_handler(buffer);
    
	//Ignore this for now
	//TODO make this work...
    if(0 && xor_error)
        return xor_error;
    
    switch(code)
    {
        case 'a': //The correct answer of a question
            spi_correct_answer(buffer);
            break;
        case 'q': //New question string
            spi_update_number_of_questions(buffer);
            break;
		
		case 's': //New time score (this is the number of points last player got)
            spi_last_score(buffer);
            break;
		//This probably means that there was no relevant data to
		//send.
        default:
            break;
	}
	
	
	//Spi is no longer busi if we get here because that means that everything went ok!
	spi_busy = 0;

    
    return 0;
}

/**
 * This function returns -1 if there is any errors in the xor byte.
 */
int spi_error_handler(const unsigned char* buffer)
{
    char xor = 0;
    int i;
    for(i = 0; i < 6; i++)
        xor ^= buffer[i];
    
    if(xor == buffer[7])
        return 0;
    else 
        return -1;
    
}

/*
 * update the correct answer
 */
void spi_correct_answer(const unsigned char* buffer)
{
	update_answer(buffer[0]);
	
}

void spi_last_score(const unsigned char* buffer)
{
	//TODO upate the last score for the player.
	
	//Create the number
	int score = buffer[0] | (buffer[1] << 8) | (buffer[2] << 16);
	
	//Update it for the main program (which waits).
	update_score_to_add(score);
}

/*
 *	Update the number of questions the pic can choose from.
 */
void spi_update_number_of_questions(const unsigned char* buffer)
{
	update_number_of_questions(buffer[0]);
	
}

void spi_request_initiation(char nof_players)
{
	char type = 'i';
	
	//i
	while(spi_busy)
		;
	
	char data[8] = {
		type,
		nof_players,
		0,0,0,0,0,type ^ nof_players
		
	};
	
	spi_dump_message(data);
	spi_request_attention();
	
}

void spi_request_show_questions_alternatives(void)
{
	char type = 't';
	
	while(spi_busy)
		;
	
	char data[8] = {
		type,
		0,
		0,0,0,0,0,type
		
	};
	
	spi_dump_message(data);
	spi_request_attention();
	
}


void spi_request_finish(void)
{
	//f
	char type = 'f';
	
	while(spi_busy)
		;
	
	char data[8] = {
		type,
		0,0,0,0,0,0,type
		
	};
	
	spi_dump_message(data);
	spi_request_attention();
}


void spi_request_next_answer(char question_id)
{
	//n
	char type = 'n';
	
	while(spi_busy)
		;
	
	char data[8] = {
		type,
		question_id,
		0,0,0,0,0,type ^ question_id
		
	};
	
	spi_dump_message(data);
	spi_request_attention();
}


void spi_send_player_answer(char player_id, char correct, char player_answer)
{
	//a
	char type = 'a';
	
	while(spi_busy)
		;
	
	char data[8] = {
		type,
		player_id,
		correct,
		player_answer,
		0,0,0,type ^ player_id ^ correct ^ player_answer
		
	};
	
	spi_dump_message(data);
	spi_request_attention();
}


void spi_request_number_of_questions(void)
{
	//q
	char type = 'q';
	
	//i
	while(spi_busy)
		;

	char data[8] = {
		type,
		0,0,0,0,0,0,type
		
	};
	
	spi_dump_message(data);	
	spi_request_attention();
}

void spi_send_question_list(char list_index)
{
	//s - type
	char type = 's';
	
	while(spi_busy)
		;
	
	char data[8] = {
		type,
		list_index,
		0,
		0,
		0,
		0,0,type ^ list_index
		
	};
	
	spi_dump_message(data);	
	spi_request_attention();
	
}

void spi_send_score(int score_1, int score_2)
{
	//u
	char type = 'u';
	
	while(spi_busy)
		;
	
	char score_as_chars[] = {
		(char) score_1 & 0xff,
		(char) (score_1 >> 8) & 0xff,
		(char) (score_1 >> 16) & 0xff,
		(char) score_2 & 0xff,
		(char) (score_2 >> 8) & 0xff,
		(char) (score_2 >> 16) & 0xff
	};
	
	char data[8] = {
		type,
		score_as_chars[0],
		score_as_chars[1],
		score_as_chars[2],
		score_as_chars[3],
		score_as_chars[4],
		score_as_chars[5],
		type ^ score_as_chars[0] ^ score_as_chars[1] ^ score_as_chars[2] ^ score_as_chars[3] ^ score_as_chars[4] ^ score_as_chars[5]
		
	};
	
	spi_dump_message(data);	
	spi_request_attention();
}

void spi_send_player_won(char player, int score)
{
	//w - type
	char type = 'w';
	
	while(spi_busy)
		;
	
	char data[8] = {
		type,
		player,
		(char) (score) & 0xff,
		(char) (score >> 8) & 0xff,
		(char) (score >> 16) & 0xff,
		0,0,type ^ player ^ ((char) (score) & 0xff) ^ ((char) (score >> 8) & 0xff) ^ ((char) (score >> 16) & 0xff)
		
	};
	
	spi_dump_message(data);	
	spi_request_attention();	
	
}











