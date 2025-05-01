/**
 * ---------------------------------------------------------------------------
 * Copyright (c) 2025 Gregory Filatov
 * All rights reserved.
 *
 * \file   leds.h
 *
 * \author Gregory Filatov
 * \date   30 April 2025
 * \version 1.1
 *
 * \brief  Contains LEDs-init example
 *
 * Module: leds.h
 *
 * Purpose: Example-file
 *
 * Functionality:
 *     Environment
 * ---------------------------------------------------------------------------
 */

#ifndef LEDS_H_
#define LEDS_H_

#include "stm32f10x.h"

#define BLUE_LED  ( GPIO_Pin_8 )
#define GREEN_LED ( GPIO_Pin_9 )

enum {
	ALT_FUNC_DISABLE = 0,
	ALT_FUNC_ENABLE = 1
};

void initLEDs(uint8_t altFunc);


#endif /* LEDS_H_ */
