/**
  ******************************************************************************
  * @file    SPI/SPI_TwoBoards/SPI_DataExchangeInterrupt/stm32f30x_it.c 
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

/** @addtogroup SPI_DataExchangeInterrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t TxBuffer[];
extern uint8_t RxBuffer[];
extern __IO uint32_t Rx_Idx;
extern __IO uint32_t Tx_Idx;

extern __IO uint32_t CmdStatus;
extern __IO uint8_t CmdTransmitted;
extern __IO uint32_t CmdReceived;

__IO uint32_t Counter = 0x00;
extern __IO uint32_t TimeOut;

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
  /* Decrement the timeout value */
  if (TimeOut != 0x0)
  {
    TimeOut--;
  }
    
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
/*                 STM32F30x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f30x.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles SPI interrupt request.
  * @param  None
  * @retval None
  */
void SPI3_IRQHandler(void)
{
#if defined (SPI_SLAVE)  
  
  /* SPI in Slave Tramitter mode--------------------------------------- */
  if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_TXE) == SET)
  {
    SPI_SendData8(SPIx, TxBuffer[Tx_Idx++]);
    if (Tx_Idx == GetVar_NbrOfData())
    {
      /* Disable the Tx buffer empty interrupt */
      SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, DISABLE);
    }
  }
  
  /* SPI in Slave Receiver mode--------------------------------------- */
  if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_RXNE) == SET)
  {
    if (CmdReceived == 0x00)
    {
      CmdReceived = SPI_ReceiveData8(SPIx);
      CmdStatus = 0x01;
    }
    else
    {
      RxBuffer[Rx_Idx++] = SPI_ReceiveData8(SPIx);
    }
  }
  
  /* SPI Error interrupt--------------------------------------- */
  if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_OVR) == SET)
  {
    SPI_ReceiveData8(SPIx);
    SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_OVR);
  }
  
#endif /* SPI_SLAVE*/
  
#if defined (SPI_MASTER)
  
  /* SPI in Master Tramitter mode--------------------------------------- */
  if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_TXE) == SET)
  {
    if (CmdStatus == 0x00)
    {
      SPI_SendData8(SPIx, CmdTransmitted);
      CmdStatus = 0x01;
    }
    else
    {
      SPI_SendData8(SPIx, TxBuffer[Tx_Idx++]);
      if (Tx_Idx == GetVar_NbrOfData())
      {
        /* Disable the Tx buffer empty interrupt */
        SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, DISABLE);
      }
    }
  }
  
  /* SPI in Master Receiver mode--------------------------------------- */
  if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_RXNE) == SET)
  {
    if (CmdReceived == 0x00)
    {
      CmdReceived = SPI_ReceiveData8(SPIx);
      Rx_Idx = 0x00;
    }
    else
    {
      RxBuffer[Rx_Idx++] = SPI_ReceiveData8(SPIx);
    }
  }
  
  /* SPI Error interrupt--------------------------------------- */
  if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_OVR) == SET)
  {
    SPI_ReceiveData8(SPIx);
    SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_OVR);
  }
  
#endif /* SPI_MASTER*/
}

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



