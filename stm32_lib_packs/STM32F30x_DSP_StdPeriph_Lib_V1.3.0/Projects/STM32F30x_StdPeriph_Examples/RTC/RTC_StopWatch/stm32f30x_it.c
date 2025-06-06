/**
  ******************************************************************************
  * @file    RTC/RTC_StopWatch/stm32f30x_it.c 
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    14-December-2021
  * @brief   Main Interrupt Service Routines.
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
#include "stm32f30x_it.h"

/** @addtogroup STM32F30x_StdPeriph_Examples
  * @{
  */

/** @addtogroup RTC_StopWatch
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE4   "    Press and hold SEL    " 
#define MESSAGE5   " to reset Backup registers" 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTC_TimeTypeDef RTC_StampTimeStruct;
extern __IO uint8_t StartEvent;
uint32_t BackupIndex = 0;
__IO uint32_t SubSecFrac = 0;
/* Define the backup register */
uint32_t BKPDataReg[10] = { RTC_BKP_DR0, RTC_BKP_DR1, RTC_BKP_DR2, RTC_BKP_DR3, RTC_BKP_DR4,
                             RTC_BKP_DR5, RTC_BKP_DR6, RTC_BKP_DR7, RTC_BKP_DR8, RTC_BKP_DR9
                          };
__IO uint32_t CurrentTimeSec = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F30x Peripherals Interrupt Handlers                   */
/******************************************************************************/
/**
* @brief  This function handles External line 5 to 9 interrupt request.
* @param  None
* @retval None
*/
void EXTI9_5_IRQHandler(void)
{
  RTC_TimeTypeDef  RTC_TimeStructureInit;
  uint16_t Colorx = 0;
  
  if((EXTI_GetITStatus(LEFT_BUTTON_EXTI_LINE) != RESET) && (StartEvent == 0x0))
  {
    /* Enable Tamper interrupt */
    RTC_ITConfig(RTC_IT_TAMP, ENABLE);
    
    /* Enable the tamper 1 */
    RTC_TamperCmd(RTC_Tamper_1, ENABLE);

    /* Set the LCD Back Color */
    LCD_SetBackColor(White);
    
    /* Get the current time */
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructureInit);
    CurrentTimeSec = (RTC_TimeStructureInit.RTC_Hours * 3600) + (RTC_TimeStructureInit.RTC_Minutes * 60) +
      RTC_TimeStructureInit.RTC_Seconds;     
    
    /* start count */
    StartEvent = 0x1;
    
    /* Clear the LEFT EXTI  pending bit */
    EXTI_ClearITPendingBit(LEFT_BUTTON_EXTI_LINE); 
  }
  
  else if((EXTI_GetITStatus(RIGHT_BUTTON_EXTI_LINE) != RESET) && (StartEvent !=0x0) && (BackupIndex < 11))
  {

      if (BackupIndex < 5)
      {
        if((uint8_t)(BackupIndex% 2) != 0x0)
        { 
          /* Set LCD backcolor*/
          LCD_SetBackColor(Blue2);
          Colorx = White;
        }
        else
        {
          /* Set LCD backcolor*/
          LCD_SetBackColor(Cyan);
          Colorx = Black;
        }
        
        RTC_GetTime(RTC_Format_BIN, &RTC_StampTimeStruct);
        
        /* Get the Current sub second and time */
        SubSecFrac = 1000 - (((uint32_t)RTC_GetSubSecond() * 1000) / 256);
        
        LCD_SetFont(&Font16x24);
        /* Display result on the LCD */
        RTC_Time_Display( LINE(3 + BackupIndex), Colorx, RTC_Get_Time(SubSecFrac , &RTC_StampTimeStruct) ); 
        
        if (BackupIndex < 2)
        {
          /* Save time register  to Backup register ( the first 5 register is reserved for time) */
          RTC_WriteBackupRegister(BKPDataReg[BackupIndex],(uint32_t)RTC->TR);
          
          /* Save sub second time stamp register ( the latest 6 register is reserved for time) */
          RTC_WriteBackupRegister(BKPDataReg[BackupIndex + 2], SubSecFrac);
        }
      }
      else
      {
        /* the backup register is full with 10trials */
        /* Set the LCD Back Color */
        LCD_SetBackColor(White);
        LCD_SetFont(&Font12x12);
        /* Set the LCD Text Color */
        LCD_SetTextColor(Red); 
        LCD_DisplayStringLine(LINE(16), (uint8_t *)MESSAGE4);
        LCD_DisplayStringLine(LINE(17), (uint8_t *)MESSAGE5);
      }  
      BackupIndex++;

    /* Set the LCD Back Color */
    LCD_SetBackColor(White); 
    /* Clear the RIGHT EXTI line */
    EXTI_ClearITPendingBit(RIGHT_BUTTON_EXTI_LINE);  
  }
  /* Clear the LEFT EXTI  pending bit */
  EXTI_ClearITPendingBit(LEFT_BUTTON_EXTI_LINE);
}

/**
* @brief  This function handles Tamper pin interrupt request.
* @param  None
* @retval None
*/
void TAMPER_STAMP_IRQHandler(void)
{
  uint8_t i =0;

  if (RTC_GetITStatus(RTC_IT_TAMP1) != RESET)
  {      
    /* Set the LCD Back Color */
    LCD_SetBackColor(White);
    LCD_SetFont(&Font16x24);
    /* Clear LCD line 5 to 9 */
    for (i=0; i < 5; i++)
    {  
      /* Clear all the LCD lines from 3 to 7 */
      LCD_ClearLine(LINE(3+i));
    }
    
    /* reset Counter */
    BackupIndex = 0 ;
    
    /* Enter to idle */
    StartEvent =0x0;
    
    RTC_Time_InitDisplay();
    
    LCD_SetFont(&Font12x12);
    LCD_ClearLine(LINE(16));
    LCD_ClearLine(LINE(17));
    
    /* Enable Tamper interrupt */
    RTC_ITConfig(RTC_IT_TAMP, DISABLE);
    
    /* Enable the tamper 1 */
    RTC_TamperCmd(RTC_Tamper_1 , DISABLE);
  }  
  /* Clear EXTI line 19 */
  EXTI_ClearITPendingBit(EXTI_Line19);
  
  /* Clear Tamper pin interrupt pending bit */
  RTC_ClearITPendingBit(RTC_IT_TAMP1);
}

/******************************************************************************/
/*                 STM32F30x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f30x.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

