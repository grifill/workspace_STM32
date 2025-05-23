/**
  ******************************************************************************
  * @file    CRC/CRC_TwoBoards/stm32f30x_it.c 
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    14-December-2021
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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

/** @addtogroup CRC_TwoBoards
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t TxBuffer[];
extern uint8_t RxBuffer[];
extern __IO uint8_t RxIndex;
extern __IO uint8_t TxIndex;
extern __IO JOYState_TypeDef PressedButton;

__IO uint8_t Counter = 0x00;
__IO uint32_t TimeOut = 0x00;  
uint8_t  ComputedCRC = 0;

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
  if (Counter < 10)
  {
    Counter++;
  }
  else
  {
    Counter = 0x00;
    STM_EVAL_LEDToggle(LED1);
  }
}
/******************************************************************************/
/*            STM32F30x Peripherals Interrupt Handlers                        */
/******************************************************************************/
/**
* @brief  This function handles USRAT interrupt request.
* @param  None
* @retval None
*/
void USARTx_IRQHandler(void)
{
#ifdef MODE_TRANSMITTER
  /* ------------------ USART in mode Tramitter ------------------------------*/
  if (USART_GetITStatus(USARTx, USART_IT_TXE) == SET)
  {
    if (TxIndex < 0x48)
    {
      /* Send TxBuffer data */
      USART_SendData(USARTx, TxBuffer[TxIndex]);
      
      /* Compute 8-bit CRC */
      CRC_CalcCRC8bits(TxBuffer[TxIndex++]);
      
    } else if (TxIndex == 0x48)
    {
      /* Get computed value */
      TxBuffer[TxIndex] = (uint8_t)CRC_GetCRC();
      
      /* Reset CRC calculation unit */
      CRC_ResetDR();
      
      /* Send CRC computed value */
      USART_SendData(USARTx, TxBuffer[TxIndex]);
      
      /* Disable the USARTx transmit data register empty interrupt */
      USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
      
      /*Discard CRC sent value*/
      TxBuffer[TxIndex] = 0;
          
      /*reset TxBuffer index*/
      TxIndex = 0;
    }
  }
#else
  /* ------------------ USART in mode Receiver -------------------------------*/
  if (USART_GetITStatus(USARTx, USART_IT_RXNE) == SET)
  {
    if (RxIndex < 0x48) 
    {
      /* Receive the USART data */    
      RxBuffer[RxIndex] = USART_ReceiveData(USARTx);
      
      /* compute 8-bit CRC */
      CRC_CalcCRC8bits(RxBuffer[RxIndex++]);
    }
    else if (RxIndex == 0x48)
    {
      /* Get computed value */
      ComputedCRC = (uint8_t)CRC_GetCRC();
      
      /* Reset CRC calculation unit */
      CRC_ResetDR();
      
      /* Receive expected CRC value */
      RxBuffer[RxIndex] = USART_ReceiveData(USARTx);
      
      /* Compare computed CRC and received CRC values*/
      if (ComputedCRC == RxBuffer[RxIndex])
      {
        /* LED3 Off */ 
        STM_EVAL_LEDOff(LED3);
        /* Toggle LED2 */ 
        STM_EVAL_LEDToggle(LED2);
      } else {
        /* LED3 On */ 
        STM_EVAL_LEDOn(LED3);
        /* LED2 Off */ 
        STM_EVAL_LEDOff(LED2);
      }
      /* Reset RxBuffer index */
      RxIndex = 0;
    }
  }
#endif /* MODE_TRANSMITTER */
}

/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @retval None
  */
#ifdef MODE_TRANSMITTER
void EXTI15_10_IRQHandler(void)
{
  /* To detect user action on JOY_SEL button */ 
  if(EXTI_GetITStatus(SEL_BUTTON_EXTI_LINE) != RESET)
  {
    PressedButton = JOY_SEL;
    /* Clear the SEL Button EXTI line pending bit */
    EXTI_ClearITPendingBit(SEL_BUTTON_EXTI_LINE);
  }
}
#endif /* MODE_TRANSMITTER */

/******************************************************************************/
/*                 STM32F30x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f30x.s).                                            */
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


