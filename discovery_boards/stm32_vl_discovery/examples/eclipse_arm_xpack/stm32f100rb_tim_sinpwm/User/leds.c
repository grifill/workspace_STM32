/**
 * ---------------------------------------------------------------------------
 * Copyright (c) 2025 Gregory Filatov
 * All rights reserved.
 *
 * \file   leds.c
 *
 * \author Gregory Filatov
 * \date   30 April 2025
 * \version 1.1
 *
 * \brief  Contains LEDs-init example
 *
 * Module: leds.c
 *
 * Purpose: Example-file
 *
 * Functionality:
 *     Environment
 * ---------------------------------------------------------------------------
 */
#include "leds.h"

void initLEDs(uint8_t altFunc) {

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	if (altFunc == ALT_FUNC_ENABLE) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	}

	GPIO_InitTypeDef port_led_init;
	port_led_init.GPIO_Pin = (BLUE_LED | GREEN_LED);
	port_led_init.GPIO_Speed = GPIO_Speed_50MHz;

	if (altFunc == ALT_FUNC_ENABLE) {
		port_led_init.GPIO_Mode = GPIO_Mode_AF_PP;
	}
	else {
		port_led_init.GPIO_Mode = GPIO_Mode_Out_PP;
	}

	GPIO_Init(GPIOC, &port_led_init);
}



