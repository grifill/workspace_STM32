/**
  ******************************************************************************
  * @file    TIM/TIM_OCActive/main.c  
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

/** @addtogroup STM32F30x_StdPeriph_Examples
  * @{
  */

/** @addtogroup TIM_OCActive
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
  
  /* TIM Configuration */
  TIM_Config();

  /* Turn on LED1 */
  STM_EVAL_LEDOn(LED1);

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Configure the TIM3 pins.
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  uint16_t CCR1_Val = 1000;
  uint16_t CCR2_Val = 500;
  uint16_t CCR3_Val = 250;
  uint16_t CCR4_Val = 125;
  uint16_t PrescalerValue = 0;
  
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
  /* GPIOA clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA |RCC_AHBPeriph_GPIOB, ENABLE);
  
  /* GPIOC Configuration: TIM3 CH1 (PA6), TIM3 CH2 (PA7),  TIM3 CH3 (PB0) and TIM3 CH4 (PB1) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  
  /* Connect TIM Channels to AF2 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_2); 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_2);
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
  TIM_OCStructInit(&TIM_OCInitStructure);
  
  /* ---------------------------------------------------------------------------
  TIM3 Configuration: Output Compare Active Mode:
  In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1)    
  TIM3CLK = 2*PCLK1  
  PCLK1 = HCLK/2 
  => TIM3CLK = HCLK = SystemCoreClock 
  
  To get TIM3 counter clock at 10 KHz, the prescaler is computed as follows:
  Prescaler = (TIM3CLK / TIM3 counter clock) - 1
  Prescaler = (SystemCoreClock /10 KHz) - 1
  
  Generate 4 signals with 4 different delays:
  TIM3_CH1 delay = CCR1_Val/TIM3 counter clock = 100 ms
  TIM3_CH2 delay = CCR2_Val/TIM3 counter clock = 50 ms
  TIM3_CH3 delay = CCR3_Val/TIM3 counter clock = 25 ms
  TIM3_CH4 delay = CCR4_Val/TIM3 counter clock = 12.5 ms
  
  Note: 
  SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f30x.c file.
  Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
  function to update SystemCoreClock variable value. Otherwise, any configuration
  based on this variable will be incorrect. 
  --------------------------------------------------------------------------- */
  
  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 10000) - 1;
  
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  /* Output Compare Active Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
  TIM_ARRPreloadConfig(TIM3, DISABLE); 
  
  /* Output Compare Active Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);
  
  /* Output Compare Active Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);
  
  /* Output Compare Active Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);
  
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);
  
  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
  
  TIM_GenerateEvent(TIM3, TIM_EventSource_Update);
  
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

/**
  * @}
  */

