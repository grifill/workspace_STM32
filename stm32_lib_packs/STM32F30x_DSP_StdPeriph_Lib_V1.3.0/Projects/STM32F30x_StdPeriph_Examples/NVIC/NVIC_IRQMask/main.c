/**
  ******************************************************************************
  * @file    NVIC/NVIC_IRQMask/main.c 
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    14-December-2021
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup NVIC_IRQMask
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void TIM_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f30x.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f30x.c file
     */
   
  /* Initialize Leds mounted on STM32303C-EVAL board */       
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  /* Initialize the KEY and SEL buttons mounted on STM32303C-EVAL board */
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_SEL, BUTTON_MODE_EXTI);
  
  /* TIM configuration -------------------------------------------------------*/
  TIM_Config();

  while (1)
  { 
    /* Wait until KEY button is pressed. */
    while(STM_EVAL_PBGetState(BUTTON_KEY) == RESET)
    {
    }
    while(STM_EVAL_PBGetState(BUTTON_KEY) != RESET)
    {
    }

    /* This instruction raises the execution priority to 0. This prevents all 
       exceptions with configurable priority from activating, other than through 
       the HardFault fault escalation mechanism. */
    __disable_irq();

    /* Turn LED4 ON */
    STM_EVAL_LEDOn(LED4);

    /* Wait until KEY button is pressed. */
    while(STM_EVAL_PBGetState(BUTTON_KEY) == RESET)
    {
    }
    while(STM_EVAL_PBGetState(BUTTON_KEY) != RESET)
    {
    }

    /* This instruction will allow all exceptions with configurable priority to 
       be activated. */
    __enable_irq();

    /* Turn LED4 OFF */
    STM_EVAL_LEDOff(LED4);
  }
}

/**
  * @brief  Configures the used Timers.
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{ 
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;

  /* Enable TIM2, TIM3 and TIM4 clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 |
                         RCC_APB1Periph_TIM4, ENABLE);

  /* TIM2 configuration */
  TIM_TimeBaseStructure.TIM_Period = 7199;
  TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/7200) - 1);
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  
  /* Init TIM_OCInitStructure */
  TIM_OCStructInit(&TIM_OCInitStructure);
  
  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_Pulse = 0x0;  
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
  
  /* TIM3 configuration */
  TIM_TimeBaseStructure.TIM_Period = 14399;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  
  /* TIM4 configuration */
  TIM_TimeBaseStructure.TIM_Period = 21599;  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);

  /* Immediate load of TIM2,TIM3 and TIM4 Precaler values */
  TIM_PrescalerConfig(TIM2, ((SystemCoreClock/7200) - 1), TIM_PSCReloadMode_Immediate);
  TIM_PrescalerConfig(TIM3, ((SystemCoreClock/7200) - 1), TIM_PSCReloadMode_Immediate);
  TIM_PrescalerConfig(TIM4, ((SystemCoreClock/7200) - 1), TIM_PSCReloadMode_Immediate);

  /* Clear TIM2, TIM3 and TIM4 update pending flags */
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);

  /* Configure two bits for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the TIM2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the TIM3 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the TIM4 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable TIM2, TIM3 and TIM4 Update interrupts */
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

  /* TIM2, TIM3 and TIM4 enable counters */
  TIM_Cmd(TIM2, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
  TIM_Cmd(TIM4, ENABLE);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


