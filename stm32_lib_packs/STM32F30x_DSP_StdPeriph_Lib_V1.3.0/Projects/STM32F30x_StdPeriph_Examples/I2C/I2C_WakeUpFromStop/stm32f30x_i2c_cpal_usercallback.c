/**
  ******************************************************************************
  * @file    I2C/I2C_WakeUpFromStop/stm32f30x_i2c_cpal_usercallback.c
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    14-December-2021
  * @brief   This file provides all the CPAL UserCallback functions.
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

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
extern __IO TestStatus TransferStatus;

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/*------------------------------------------------------------------------------
                     CPAL User Callbacks implementations 
------------------------------------------------------------------------------*/


/*=========== Timeout UserCallback ===========*/


/**
  * @brief  User callback that manages the Timeout error
  * @param  pDevInitStruct
  * @retval None.
  */
uint32_t CPAL_TIMEOUT_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
  /* Update LCD Display */
  LCD_SetBackColor(Red);
  LCD_SetTextColor(White); 
  LCD_DisplayStringLine(Line8, MESSAGE12);
  
  /* Update CPAL_State, wCPAL_DevError and wCPAL_Timeout */ 
  pDevInitStruct->CPAL_State = CPAL_STATE_READY;
  pDevInitStruct->wCPAL_DevError = CPAL_I2C_ERR_NONE ;
  pDevInitStruct->wCPAL_Timeout  = CPAL_I2C_TIMEOUT_DEFAULT; 
  
  /* DeInitialize CPAL device */
  CPAL_I2C_DeInit(pDevInitStruct);  
  
  /* Initialize CPAL device with the selected parameters */
  CPAL_I2C_Init(pDevInitStruct);

  return CPAL_PASS; 
}


/*=========== Transfer UserCallback ===========*/

/**
  * @brief  Manages the End of Tx transfer event
  * @param  pDevInitStruct 
  * @retval None
  */
/*void CPAL_I2C_TXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{

}*/


/**
  * @brief  Manages the End of Rx transfer event
  * @param  pDevInitStruct
  * @retval None
  */
/*void CPAL_I2C_RXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{


}*/

/**
  * @brief  Manages Tx transfer event
  * @param  pDevInitStruct
  * @retval None
  */
/*void CPAL_I2C_TX_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{

}*/


/**
  * @brief  Manages Rx transfer event
  * @param  pDevInitStruct
  * @retval None
  */
/*void CPAL_I2C_RX_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{

}*/


/**
  * @brief  Manages the End of DMA Tx transfer event
  * @param  pDevInitStruct
  * @retval None
  */
/*void CPAL_I2C_DMATXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{

}*/


/**
  * @brief  Manages the Half of DMA Tx transfer event
  * @param  pDevInitStruct
  * @retval None
  */
/*void CPAL_I2C_DMATXHT_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{

}*/


/**
  * @brief  Manages Error of DMA Tx transfer event
  * @param  pDevInitStruct
  * @retval None
  */
/*void CPAL_I2C_DMATXTE_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{

}*/


/**
  * @brief  Manages the End of DMA Rx transfer event
  * @param  pDevInitStruct
  * @retval None
  */
/*void CPAL_I2C_DMARXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{

}*/


/**
  * @brief  Manages the Half of DMA Rx transfer event
  * @param  pDevInitStruct
  * @retval None
  */
/*void CPAL_I2C_DMARXHT_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{

}*/


/**
  * @brief  Manages Error of DMA Rx transfer event
  * @param  pDevInitStruct
  * @retval None
  */
/*void CPAL_I2C_DMARXTE_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{

}*/


/*=========== Error UserCallback ===========*/


/**
  * @brief  User callback that manages the I2C device errors.
  * @note   Make sure that the define USE_SINGLE_ERROR_CALLBACK is uncommented in
  *         the cpal_conf.h file, otherwise this callback will not be functional.
  * @param  pDevInitStruct. 
  * @param  DeviceError.
  * @retval None
  */ 
void CPAL_I2C_ERR_UserCallback(CPAL_DevTypeDef pDevInstance, uint32_t DeviceError)
{ 
  /* Update CPAL_State */
  I2C_DevStructures[pDevInstance]->CPAL_State = CPAL_STATE_READY;
  
  /* Update Display settings */
  LCD_SetBackColor(Red);
  LCD_SetTextColor(White); 
  
  /* if an error occurred except AF*/
  if (DeviceError != CPAL_I2C_ERR_AF )
  {
    /* Deinitialize device */
    CPAL_I2C_DeInit(I2C_DevStructures[pDevInstance]);
    
    /* Initialize device with the selected parameters */
    CPAL_I2C_Init(I2C_DevStructures[pDevInstance]);   
    
    /* Update LCD Display */
    LCD_DisplayStringLine(Line8, MESSAGE11);    
  }
  else
  {
    /* Update LCD Display */
    LCD_DisplayStringLine(Line8, MESSAGE10);
  }
  
  /* Update wCPAL_DevError */  

  I2C_DevStructures[pDevInstance]->wCPAL_DevError = CPAL_I2C_ERR_NONE ;
  
  TransferStatus = FAILED;
}

/**
  * @brief  User callback that manages BERR I2C device errors
  * @note   Make sure that the define USE_MULTIPLE_ERROR_CALLBACK is uncommented in
  *         the cpal_conf.h file, otherwise this callback will not be functional
  * @param  pDevInstance
  * @retval None
  */
/*void CPAL_I2C_BERR_UserCallback(CPAL_DevTypeDef pDevInstance)
{

}*/


/**
  * @brief  User callback that manages ARLO I2C device errors
  * @note   Make sure that the define USE_MULTIPLE_ERROR_CALLBACK is uncommented in
  *         the cpal_conf.h file, otherwise this callback will not be functional
  * @param  pDevInstance
  * @retval None
  */
/*void CPAL_I2C_ARLO_UserCallback(CPAL_DevTypeDef pDevInstance)
{

}*/


/**
  * @brief  User callback that manages OVR I2C device errors
  * @note   Make sure that the define USE_MULTIPLE_ERROR_CALLBACK is uncommented in
  *         the cpal_conf.h file, otherwise this callback will not be functional
  * @param  pDevInstance
  * @retval None
  */
/*void CPAL_I2C_OVR_UserCallback(CPAL_DevTypeDef pDevInstance)
{

}*/


/**
  * @brief  User callback that manages AF I2C device errors.
  * @note   Make sure that the define USE_MULTIPLE_ERROR_CALLBACK is uncommented in
  *         the cpal_conf.h file, otherwise this callback will not be functional
  * @param  pDevInstance
  * @retval None
  */
/*void CPAL_I2C_AF_UserCallback(CPAL_DevTypeDef pDevInstance)
{

}*/


/*=========== Addressing Mode UserCallback ===========*/


/**
  * @brief  User callback that manage General Call Addressing mode
  * @param  pDevInitStruct
  * @retval None
  */
/*void CPAL_I2C_GENCALL_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{

}*/


/**
  * @brief  User callback that manage Dual Address Addressing mode
  * @param  pDevInitStruct
  * @retval None
  */
/*void CPAL_I2C_DUALF_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{

}*/


