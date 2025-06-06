/**
  ******************************************************************************
  * @file    COMP/COMP_AnalogWatchdog/main.c 
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

/** @addtogroup COMP_AnalogWatchdog
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t State = 0;

/* Private function prototypes -----------------------------------------------*/
static void COMP_Config(void);
static void StopSequence_Config(void);
static void Configuration_Restore(void);

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

  /******* Initialize LEDs available on STM32303C-EVAL board ******************/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  /* configure COMP1 and COMP2 with interrupts enabled */
  COMP_Config();

  /* Check input voltage level: within the thresholds, above the upper threshold
     or under the lower threshold */
  InputVoltageLevel_Check();
  
  /* Infinite loop */
  while (1)
  {
    if (State == STATE_OVER_THRESHOLD)
    {
      /* Restore config: clock, GPIO... */
      Configuration_Restore();
  
      /* Restore GPIO configuration */
      STM_EVAL_LEDInit(LED1);
      STM_EVAL_LEDInit(LED2);
      STM_EVAL_LEDInit(LED3);
      STM_EVAL_LEDInit(LED4);

      /* Turn on LD1 and LD3 and turn off LD2 and LD4 */
      STM_EVAL_LEDOn(LED1);
      STM_EVAL_LEDOff(LED2);
      STM_EVAL_LEDOn(LED3);
      STM_EVAL_LEDOff(LED4);
      
      while(State == STATE_OVER_THRESHOLD)
      {
        /* add your code here */
      }
    }
    else if (State == STATE_WITHIN_THRESHOLD)
    {
      /* Input voltage is within the thresholds: higher and lower thresholds */
      /* Enter STOP mode with regulator in low power */
      StopSequence_Config();
    }
    else /* (State == STATE_UNDER_THRESHOLD) */
    {
      /* Restore config: clock, GPIO... */
      Configuration_Restore();

      /* Restore GPIO configuration */
      STM_EVAL_LEDInit(LED1);
      STM_EVAL_LEDInit(LED2);
      STM_EVAL_LEDInit(LED3);
      STM_EVAL_LEDInit(LED4);

      /* Turn on LD2 & LD4 and turn off LD1 & LD3 */
      STM_EVAL_LEDOff(LED1);
      STM_EVAL_LEDOn(LED2);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOn(LED4);

      while(State == STATE_UNDER_THRESHOLD)
      {
         /* add your code here */
      }
    }
  }
}

/**
  * @brief  Configure COMP1 and COMP2 with interrupt
  * @param  None
  * @retval None
  */
static void COMP_Config(void)
{
  COMP_InitTypeDef        COMP_InitStructure;
  EXTI_InitTypeDef        EXTI_InitStructure;
  NVIC_InitTypeDef        NVIC_InitStructure;
  GPIO_InitTypeDef        GPIO_InitStructure;
  
  /* GPIOA Peripheral clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure PA1: PA1 is used as COMP1 and COMP2 non inveting input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* COMP Peripheral clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* COMP1 Init: the higher threshold is set to VREFINT ~ 1.22V
     but can be changed to other available possibilities */
  COMP_StructInit(&COMP_InitStructure);
  COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_VREFINT;
  COMP_InitStructure.COMP_Output = COMP_Output_None;
  COMP_InitStructure.COMP_Mode = COMP_Mode_LowPower;
  COMP_InitStructure.COMP_Hysteresis = COMP_Hysteresis_High;
  COMP_Init(COMP_Selection_COMP1, &COMP_InitStructure);

  /* COMP2 Init: the lower threshold is set to VREFINT/4 ~ 1.22 / 4 ~ 0.305 V 
     but can be changed to other available possibilities */
  COMP_StructInit(&COMP_InitStructure);
  COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_1_4VREFINT;
  COMP_InitStructure.COMP_Output = COMP_Output_None;
  COMP_InitStructure.COMP_Mode = COMP_Mode_LowPower;
  COMP_InitStructure.COMP_Hysteresis = COMP_Hysteresis_High;
  COMP_Init(COMP_Selection_COMP2, &COMP_InitStructure);

  /* Enable Window mode */
  COMP_WindowCmd(COMP_Selection_COMP2, ENABLE);
  
  /* Enable COMP1: the higher threshold is set to VREFINT ~ 1.22 V */
  COMP_Cmd(COMP_Selection_COMP1, ENABLE);
  /* Enable COMP2: the lower threshold is set to VREFINT/4 ~ 0.305 V */
  COMP_Cmd(COMP_Selection_COMP2, ENABLE);

  /* Configure EXTI Line 21 in interrupt mode */
  EXTI_InitStructure.EXTI_Line = EXTI_Line21;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Configure EXTI Line 22 in interrupt mode */
  EXTI_InitStructure.EXTI_Line = EXTI_Line22;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Clear EXTI21 line */
  EXTI_ClearITPendingBit(EXTI_Line21);

  /* Clear EXTI22 line */
  EXTI_ClearITPendingBit(EXTI_Line22);

  /* Configure COMP IRQ */
  NVIC_InitStructure.NVIC_IRQChannel = COMP1_2_3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Prepare the system to enter STOP mode.
  * @param  None
  * @retval None
  */
static void StopSequence_Config(void)
{
  GPIO_InitTypeDef        GPIO_InitStructure;
  
  /* PWR Peripheral clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
  /* Enable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOF, ENABLE);

  /* Configure all GPIO port pins in Analog mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  
  /* Request to enter STOP mode with regulator in low power */
  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}

/**
  * @brief  check input voltage level: within the thresholds, above the upper 
  *         threshold or under the lower threshold
  * @param  None
  * @retval None
  */
void InputVoltageLevel_Check(void)
{
  /*  Check if COMP1 and COMP2 output level is high */
  if ((COMP_GetOutputLevel(COMP_Selection_COMP1) == COMP_OutputLevel_High) 
   && (COMP_GetOutputLevel(COMP_Selection_COMP2) == COMP_OutputLevel_High))
  {
    /* A rising edge is detected so the input voltage is higher than VREFINT */
    State = STATE_OVER_THRESHOLD;
  }
  else if ((COMP_GetOutputLevel(COMP_Selection_COMP1) == COMP_OutputLevel_Low)
       && (COMP_GetOutputLevel(COMP_Selection_COMP2) == COMP_OutputLevel_High))
  {
    /* A falling edge is detected so the input voltage is lower than VREFINT */
    State = STATE_WITHIN_THRESHOLD;
  }
  else if ((COMP_GetOutputLevel(COMP_Selection_COMP1) == COMP_OutputLevel_Low)
       && (COMP_GetOutputLevel(COMP_Selection_COMP2) == COMP_OutputLevel_Low))
  {
    State = STATE_UNDER_THRESHOLD;
  }
}

/**
  * @brief  Restore peripheral config before entering STOP mode.
  * @param  None
  * @retval None
  */
static void Configuration_Restore(void)
{
  __IO uint32_t HSEStatus = 0;
  
  /* SYSCLK, HCLK, PCLK configuration ----------------------------------------*/    
  /* Enable HSE */    
  RCC_HSEConfig(RCC_HSE_ON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  HSEStatus = RCC_WaitForHSEStartUp();

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Enable Prefetch Buffer */
    FLASH_SetLatency(FLASH_Latency_1);
    
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
   
    /* PCLK = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /*  PLL configuration:  = HSE *  9 = 72 MHz */
    RCC_PREDIV1Config(RCC_PREDIV1_Div1);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
    
    /* Enable PLL */
    RCC_PLLCmd(ENABLE);
    
    /* PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
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

