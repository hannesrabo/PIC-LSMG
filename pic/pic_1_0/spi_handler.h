/*
 * Header to the main file handeling full spi messages.
 * 
 * Copyright (c) 2016 Hannes Rabo
 */

#ifndef GUARD_game_logic
#define GUARD_game_logic

#include <pic32mx.h>
#include "game.h"
#include "random.h"
#include "globals.h"
#include "spi.h"

int spi_message_handler(const unsigned char* buffer);
int spi_error_handler(const unsigned char* buffer);

void spi_correct_answer(const unsigned char* buffer);
void spi_update_number_of_questions(const unsigned char* buffer);
void spi_last_score(const unsigned char* buffer);

void spi_request_initiation(char nof_players);
void spi_request_finish(void);
void spi_request_next_answer(char question_id);
void spi_send_player_answer(char player_id, char correct, char player_answer);
void spi_request_number_of_questions(void);
void spi_send_score(int score_1, int score_2);
void spi_send_player_won(char player, int score);
void spi_send_question_list(char list_index);
void spi_request_show_questions_alternatives(void);

#endif //GUARD_game_logic
