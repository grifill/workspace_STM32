/**
  ******************************************************************************
  * @file    PWR/PWR_CurrentConsumption/main.c 
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

/** @addtogroup PWR_CurrentConsumption
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t i = 0;
__IO uint8_t KeyPressed = 0;
/* Private function prototypes -----------------------------------------------*/
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
  
  /* Configure Key Button */
  STM_EVAL_PBInit(BUTTON_KEY,BUTTON_MODE_GPIO);
    
  /* Loop while KEY button is maintained pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) != RESET)
  {
  }
    
  /* Enable PWR APB1 Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    
  /* Allow access to Backup */
  PWR_BackupAccessCmd(ENABLE);
    
  /* Reset RTC Domain */
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);
    
  /* Loop while KEY button is maintained pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) != RESET)
  {
  }
  /* Loop while KEY button is maintained pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) == RESET)
  {
  }
      
#if defined (SLEEP_MODE)
  /* Sleep Mode Entry 
      - System Running at PLL (72MHz)
      - Flash 2 wait state
      - Prefetch and Cache enabled
      - Code running from Internal FLASH
      - All peripherals disabled.
      - Wakeup using EXTI Line (USER Button PE.06)
   */  
  SleepMode_Measure();
#elif defined (STOP_MODE)
  /* STOP Mode Entry 
      - RTC Clocked by LSI
      - Regulator in LP mode
      - HSI, HSE OFF and LSI OFF if not used as RTC Clock source  
      - No IWDG
      - FLASH in deep power down mode
      - Automatic Wakeup using RTC clocked by LSI 
   */
  /* When using the small packages (48 and 64 pin packages), the GPIO pins which 
     are not present on these packages, must not be configured in analog mode.*/
  StopMode_Measure();
#elif defined (STANDBY_MODE)
  /* STANDBY Mode Entry 
      - RTC OFF
      - IWDG and LSI OFF
      - Wakeup using WakeUp Pin (PC.13)
   */
  StandbyMode_Measure();
#elif defined (STANDBY_RTC_MODE)
  /* STANDBY Mode with RTC on LSI Entry 
      - RTC Clocked by LSI
      - IWDG OFF and LSI OFF if not used as RTC Clock source
      - Automatic Wakeup using RTC clocked by LSI 
   */

  StandbyRTCMode_Measure();
#else

  /* Initialize LED3 on STM32303C-EVAL board */
  STM_EVAL_LEDInit(LED3);
  
  /* Infinite loop */
  while (1)
  {
    /* Toggle The LED3 */
    STM_EVAL_LEDToggle(LED3);

    /* Inserted Delay */
    for(i = 0; i < 0x7FFF; i++);
  }
#endif
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

