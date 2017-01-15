/*
 * Header for main event handler.
 * 
 * Copyright (c) 2016 Hannes Rabo
 * Copyright (c) 2016 Julius Celik
 */

#ifndef GUARD_isr
#define GUARD_isr

#include <pic32mx.h>
#include "spi.h"
#include "button_handler.h"

void usr_isr(void);
extern void enable_interrupt();

#endif //GUARD_isr
