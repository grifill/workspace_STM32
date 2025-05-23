/**
  ******************************************************************************
  * @file    TIM/TIM_DMABurst/main.c  
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
#include "stm32f30x.h"

/** @addtogroup STM32F30x_StdPeriph_Examples
  * @{
  */

/** @addtogroup TIM_DMABurst
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
#define TIM1_DMAR_ADDRESS ((uint32_t)0x40012C4C) /* TIM DMAR address */
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

  /* TIM1 Configuration */
  TIM_Config();       

  /* Infinite loop */ 
  while(1)
  {
  }
}

/**
  * @brief  Configure the TIM Pins.
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  GPIO_InitTypeDef         GPIO_InitStructure;
  DMA_InitTypeDef          DMA_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef        TIM_OCInitStructure;
  uint16_t SRC_Buffer[3] = {0x0FFF, 0x0000, 0x0555};
  
  /* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  /* GPIOA clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
  /* GPIOA Configuration: PA8(TIM1 CH1) as alternate function push-pull ------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Connect TIM pins to AF6 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_6);
  
  /* DeInitialize the DMA1 channel5 */
  DMA_DeInit(DMA1_Channel5);

  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)TIM1_DMAR_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SRC_Buffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  
  DMA_InitStructure.DMA_BufferSize = 3;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel5, &DMA_InitStructure);
  
    
  /* Time base configuration */
  /* -----------------------------------------------------------------------
    TIM1 Configuration: generate 1 PWM signal using the DMA burst mode:
   
    TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2) 
      TIM1CLK = PCLK2  
      PCLK2 = HCLK  
      => TIM1CLK = HCLK = SystemCoreClock
    
    To get TIM1 counter clock at 36 MHz, the prescaler is computed as follows:
      Prescaler = (TIM1CLK / TIM1 counter clock) - 1
      Prescaler = (SystemCoreClock /36 MHz) - 1
  
   TIM1 Frequency = TIM1 counter clock/(ARR + 1)
                                               = 36 MHz / 4096 = 8.79 KHz
    TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR)* 100 = 33.33%
  
    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f30x.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.  
  ----------------------------------------------------------------------- */  
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;          
  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) (SystemCoreClock / 36000000) - 1;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Init OC structure */
  TIM_OCStructInit(&TIM_OCInitStructure);
  
  /* TIM Configuration in PWM Mode */
  TIM_OCInitStructure.TIM_OCMode =  TIM_OCMode_PWM1;    
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;          
  TIM_OCInitStructure.TIM_Pulse = 0xFFF;  
  TIM_OC1Init(TIM1, &TIM_OCInitStructure); 

  /* TIM1 DMAR Base register and DMA Burst Length Config */
  TIM_DMAConfig(TIM1, TIM_DMABase_ARR, TIM_DMABurstLength_3Transfers);

  /* TIM1 DMA Update enable */
  TIM_DMACmd(TIM1, TIM_DMA_Update, ENABLE);

  /* TIM1 PWM Outputs Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  
  /* TIM1 enable */
  TIM_Cmd(TIM1, ENABLE);
  
  /* Enable DMA1 channel5  */
  DMA_Cmd(DMA1_Channel5, ENABLE);

  /* Wait until DMA1 channel5 end of Transfer */
  while (!DMA_GetFlagStatus(DMA1_FLAG_TC5))
  {
  }
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

