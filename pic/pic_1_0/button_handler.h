/*
 * Header for button control
 * 
 * Copyright (c) 2016 Hannes Rabo
 */

#ifndef GUARD_button_handler
#define GUARD_button_handler

#include <pic32mx.h>
#include "game.h"

#define BUTTONS_CONNECTED	3

int buttons_init(void);					//Initiate whats needed for the buttons
char cycle_current(char player_id, char button_id);		//...
int poll_button();					//This is not automatically called when a button is pressed.

#endif	//GUARD_button_handler
