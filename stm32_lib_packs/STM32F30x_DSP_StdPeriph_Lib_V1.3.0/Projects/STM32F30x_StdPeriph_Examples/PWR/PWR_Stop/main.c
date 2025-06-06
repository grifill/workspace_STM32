/**
  ******************************************************************************
  * @file    PWR/PWR_Stop/main.c 
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
#include "stm32303c_eval.h"

/** @addtogroup STM32F30x_Peripheral_Examples
  * @{
  */

/** @addtogroup PWR_STOP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t TimingDelay;

/* Private function prototypes -----------------------------------------------*/
static void RTC_Config(void);
static void SYSCLKConfig_STOP(void);
static void RTC_AlarmConfig(void);
static void SysTick_Configuration(void);
void Delay(__IO uint32_t nTime);
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
     
  /* Configures the Key button */
  STM_EVAL_PBInit(BUTTON_KEY,BUTTON_MODE_EXTI);
  
  /* Configure LED1, LED2, LED3 and LED4 LEDs */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  /* Configures systick */
  SysTick_Configuration();
  
  /* RTC Configuration */
  RTC_Config();
   
  /* LED1 On */
  STM_EVAL_LEDOn(LED1);
  
  while(1)
  {
    /* Insert 2 second delay */
    Delay(10);
    
    /* RTC Configuration */
    RTC_AlarmConfig();
    
    /* LEDs Off */
    STM_EVAL_LEDOff(LED1);
    STM_EVAL_LEDOff(LED2);
    STM_EVAL_LEDOff(LED4);
    
    /* Request to enter STOP mode with regulator in low power mode */
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
    
    /* LED1 On */
    STM_EVAL_LEDOn(LED1);
    
    /* Disable the RTC Alarm interrupt */
    RTC_ITConfig(RTC_IT_ALRA, DISABLE);
    RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
    
    /* Configures system clock after wake-up from STOP */
    SYSCLKConfig_STOP();
  }
}

/**
  * @brief  Configures the RTC clock source.
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  RTC_TimeTypeDef   RTC_TimeStructure;
  RTC_InitTypeDef   RTC_InitStructure;
  
  /* RTC Configuration **********************************************************/ 
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset back up registers */
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);
  
  /* Enable the LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}
  
  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);
  
  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();  
  
  /* Set RTC calendar clock to 1 HZ (1 second) */
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
  RTC_InitStructure.RTC_SynchPrediv = 0x0FF;
  
  if (RTC_Init(&RTC_InitStructure) == ERROR)
  {
    while(1);
  }

  /* Set the time to 01h 00mn 00s AM */
  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = 0x01;
  RTC_TimeStructure.RTC_Minutes = 0x00;
  RTC_TimeStructure.RTC_Seconds = 0x00;  
  
  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
}

/**
  * @brief  Configures the RTC clock source.
  * @param  None
  * @retval None
  */
static void RTC_AlarmConfig(void)
{  
  RTC_TimeTypeDef   RTC_TimeStructure;
  RTC_AlarmTypeDef  RTC_AlarmStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* EXTI configuration */
  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Alarm Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Get current time */
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);

  /* Set the alarm to current time + 5s */
  RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = RTC_TimeStructure.RTC_Hours;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = RTC_TimeStructure.RTC_Minutes;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = RTC_TimeStructure.RTC_Seconds + 5;
  RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
  RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay | RTC_AlarmMask_Minutes |
                                     RTC_AlarmMask_Hours;
  RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
   
  /* Enable the RTC Alarm A interrupt */
  RTC_ITConfig(RTC_IT_ALRA, ENABLE);

  /* Enable the alarm */
  RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
    
  /* Clear the Alarm A Pending Bit */
  RTC_ClearITPendingBit(RTC_IT_ALRA);  
}

/**
  * @brief  Configures system clock after wake-up from STOP: enable HSE, PLL
  *         and select PLL as system clock source.
  * @param  None
  * @retval None
  */
static void SYSCLKConfig_STOP(void)
{  
  /* After wake-up from STOP reconfigure the system clock */
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  
  /* Wait till HSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {}
  
  /* Enable PLL */
  RCC_PLLCmd(ENABLE);
  
  /* Wait till PLL is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  {}
  
  /* Select PLL as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
  /* Wait till PLL is used as system clock source */
  while (RCC_GetSYSCLKSource() != 0x08)
  {}
}


/**
  * @brief  Configures the SysTick to generate an interrupt each 250 millisecond.
  * @param  None
  * @retval None
  */
static void SysTick_Configuration(void)
{
  /* SysTick interrupt each 250 ms */
  if (SysTick_Config((SystemCoreClock/8) / 4))
  { 
    /* Capture error */ 
    while (1);
  }

  /* Select AHB clock(HCLK) divided by 8 as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

  /* Set SysTick Preemption Priority to 1 */
  NVIC_SetPriority(SysTick_IRQn, 0x04);
}


/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, with a base of 250 milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);

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

