/**
 * ---------------------------------------------------------------------------
 * Copyright (c) 2025 Gregory Filatov
 * All rights reserved.
 *
 * \file   main.c
 *
 * \author Gregory Filatov
 * \date   29 April 2025
 * \version 1.0
 *
 * \brief  Contains stm32f100rb_tim_sinpwm example
 *
 * Module: main.c
 *
 * Purpose: Example-file
 *
 * Functionality:
 *     Environment
 * ---------------------------------------------------------------------------
 */
#include "stm32f10x.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>

#include <misc.h>
#include <stm32f10x_conf.h>

extern void TIM3_IRQHandler(void);

int iter;
int occ_cnt[64] = { 490, 980, 1467, 1950, 2429, 2902, 3368, 3826,
		           4275, 4713, 5141, 5555, 5956, 6343, 6715, 7071,
				   7409, 7730, 8032, 8314, 8577, 8819, 9039, 9238,
				   9415, 9569, 9700, 9807, 9891, 9951, 9987, 10000,
				   9987, 9951, 9891, 9807, 9700, 9569, 9415, 9238,
				   9039, 8819, 8577, 8314, 8032, 7730, 7409, 7071,
				   6715, 6343, 5956, 5555, 5141, 4713, 4275, 3826,
				   3368, 2902, 2429, 1950, 1467, 980, 490, 0 };

int main(void){

	iter = 0;

	// Init GPIO ------------------------------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef port_init;
	port_init.GPIO_Pin = (GPIO_Pin_8 | GPIO_Pin_9);
	port_init.GPIO_Speed = GPIO_Speed_50MHz;
	port_init.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &port_init);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

	// Init Timer ----------------------------------------
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef base_timer;
	TIM_TimeBaseStructInit(&base_timer);
	base_timer.TIM_Prescaler = 120;
	base_timer.TIM_Period = 10001;
	base_timer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &base_timer);

	TIM_OCInitTypeDef tim_init;
	TIM_OCStructInit(&tim_init);
	tim_init.TIM_OCMode = TIM_OCMode_PWM1;
	tim_init.TIM_OutputState = TIM_OutputState_Enable;
	tim_init.TIM_Pulse = 50;
	tim_init.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM3, &tim_init);

	TIM_OC3Init(TIM3, &tim_init);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_OC4Init(TIM3,&tim_init);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);

	// Interrupt config ---------------------------------
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig((TIM_TypeDef *)TIM3_BASE,TIM_IT_Update, ENABLE);
	TIM_ClearITPendingBit((TIM_TypeDef *)TIM3_BASE, TIM_IT_Update);

	// Start
	TIM_Cmd(TIM3, ENABLE);


	// Main cycle
    while (1){
    }

    return 0;
}

// IRQ ------------------------------------------------
void TIM3_IRQHandler(void)
{
	switch (TIM_GetITStatus_IT(TIM3, TIM_IT_Update))
	{
	case 0:
		break;
	default:
	{
		switch(iter/64)
		{
		case 0:
			TIM3->CCR3 = (uint16_t)occ_cnt[iter];
			TIM3->CCR4 = (uint16_t)occ_cnt[iter];
			iter+=1;
			break;
		default:
			switch(iter/128)
			{
			case 0:
				//TIM3->CCR3 = 0;
				iter+=1;
				break;
			default:
				//TIM3->CCR3 = 0;
				iter = 0;
				break;
			}
			break;
		}
	}
	TIM_ClearITPendingBit_M(TIM3, TIM_IT_Update);
	}
}


















