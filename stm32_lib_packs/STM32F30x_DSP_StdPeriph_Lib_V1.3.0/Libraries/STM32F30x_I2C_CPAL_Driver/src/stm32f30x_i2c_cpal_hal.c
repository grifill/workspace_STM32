/**
  *********************************************************************************
  * @file    stm32f30x_i2c_cpal_hal.c
  * @author  MCD Application Team
  * @version V1.2.2
  * @date    15-December-2021
  * @brief   This file provides all the CPAL_I2C_HAL (hardware Abstraction Layer)
  *          firmware functions.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_i2c_cpal_hal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

/*========= Local Structures declaration =========*/

CPAL_InitTypeDef* I2C_DevStructures[CPAL_I2C_DEV_NUM] =
{
#ifdef CPAL_I2C1_SUPPORTED
  #ifdef CPAL_USE_I2C1
    &I2C1_DevStructure,
  #else
    pNULL,
  #endif
#endif /* CPAL_I2C1_SUPPORTED */

#ifdef CPAL_I2C2_SUPPORTED
  #ifdef CPAL_USE_I2C2
    &I2C2_DevStructure,
  #else
    pNULL,
  #endif
#endif /* CPAL_I2C2_SUPPORTED */

#ifdef CPAL_I2C3_SUPPORTED
  #ifdef CPAL_USE_I2C3
    &I2C3_DevStructure,
  #else
    pNULL,
  #endif
#endif /* CPAL_I2C3_SUPPORTED */
};

#ifdef CPAL_I2C1_SUPPORTED
  #ifdef CPAL_USE_I2C1
  CPAL_InitTypeDef I2C1_DevStructure = {  CPAL_I2C1,                          /* I2C1 device number */
                                          CPAL_DIRECTION_TXRX,                /* Transmitter and Receiver direction selected */
                                          CPAL_MODE_MASTER,                   /* Mode Master selected */
                                          #ifdef CPAL_I2C_DMA_PROGMODEL
                                          CPAL_PROGMODEL_DMA,                 /* DMA Programming Model selected */
                                          #else
                                          CPAL_PROGMODEL_INTERRUPT,           /* IT Programming Model selected */
                                          #endif /* CPAL_I2C_DMA_PROGMODEL */
                                          (CPAL_TransferTypeDef*) pNULL,       /* Point pCPAL_TransferTx to a Null pointer */
                                          (CPAL_TransferTypeDef*) pNULL,       /* Point pCPAL_TransferRx to a Null pointer */
                                          CPAL_STATE_DISABLED,                 /* Device Disabled */
                                          CPAL_I2C_ERR_NONE,                   /* No Device Error */
                                          ((uint32_t)0x00000000),              /* No Options selected */
                                          ((uint32_t)CPAL_I2C_TIMEOUT_DEFAULT),/* Set timeout value to CPAL_I2C_TIMEOUT_DEFAULT */
                                          (I2C_InitTypeDef*) pNULL};           /* Point pCPAL_I2C_Struct to a Null pointer */

  #endif /* CPAL_USE_I2C1 */
#endif /* CPAL_I2C1_SUPPORTED */

#ifdef CPAL_I2C2_SUPPORTED
  #ifdef CPAL_USE_I2C2
  CPAL_InitTypeDef I2C2_DevStructure = {  CPAL_I2C2,                          /* I2C2 device number */
                                          CPAL_DIRECTION_TXRX,                /* Transmitter and Receiver direction selected */
                                          CPAL_MODE_MASTER,                   /* Mode Master selected */
                                          #ifdef CPAL_I2C_DMA_PROGMODEL
                                          CPAL_PROGMODEL_DMA,                 /* DMA Programming Model selected */
                                          #else
                                          CPAL_PROGMODEL_INTERRUPT,           /* IT Programming Model selected */
                                          #endif /* CPAL_I2C_DMA_PROGMODEL */
                                          (CPAL_TransferTypeDef*) pNULL,       /* Point pCPAL_TransferTx to a Null pointer */
                                          (CPAL_TransferTypeDef*) pNULL,       /* Point pCPAL_TransferRx to a Null pointer */
                                          CPAL_STATE_DISABLED,                 /* Device Disabled */
                                          CPAL_I2C_ERR_NONE,                   /* No Device Error */
                                          ((uint32_t)0x00000000),              /* No Options selected */
                                          ((uint32_t)CPAL_I2C_TIMEOUT_DEFAULT),/* Set timeout value to CPAL_I2C_TIMEOUT_DEFAULT */
                                          (I2C_InitTypeDef*) pNULL};           /* Point pCPAL_I2C_Struct to a Null pointer */
  #endif /* CPAL_USE_I2C2 */
#endif /* CPAL_I2C2_SUPPORTED */

#ifdef CPAL_I2C3_SUPPORTED
  #ifdef CPAL_USE_I2C3
  CPAL_InitTypeDef I2C3_DevStructure = {  CPAL_I2C3,                          /* I2C3 device number */
                                          CPAL_DIRECTION_TXRX,                /* Transmitter and Receiver direction selected */
                                          CPAL_MODE_MASTER,                   /* Mode Master selected */
                                          #ifdef CPAL_I2C_DMA_PROGMODEL
                                          CPAL_PROGMODEL_DMA,                 /* DMA Programming Model selected */
                                          #else
                                          CPAL_PROGMODEL_INTERRUPT,           /* IT Programming Model selected */
                                          #endif /* CPAL_I2C_DMA_PROGMODEL */
                                          (CPAL_TransferTypeDef*) pNULL,       /* Point pCPAL_TransferTx to a Null pointer */
                                          (CPAL_TransferTypeDef*) pNULL,       /* Point pCPAL_TransferRx to a Null pointer */
                                          CPAL_STATE_DISABLED,                 /* Device Disabled */
                                          CPAL_I2C_ERR_NONE,                   /* No Device Error */
                                          ((uint32_t)0x00000000),              /* No Options selected */
                                          ((uint32_t)CPAL_I2C_TIMEOUT_DEFAULT),/* Set timeout value to CPAL_I2C_TIMEOUT_DEFAULT */
                                          (I2C_InitTypeDef*) pNULL};           /* Point pCPAL_I2C_Struct to a Null pointer */
  #endif /* CPAL_USE_I2C3 */
#endif /* CPAL_I2C3_SUPPORTED */

DMA_InitTypeDef CPAL_DMA_InitStructure;

#if defined (CPAL_I2C1_SUPPORTED) && defined (CPAL_I2C2_SUPPORTED) && defined (CPAL_I2C3_SUPPORTED)

I2C_TypeDef* CPAL_I2C_DEVICE[3] = {I2C1,I2C2,I2C3};

const uint32_t CPAL_I2C_CLK[3] = {CPAL_I2C1_CLK,CPAL_I2C2_CLK,CPAL_I2C3_CLK};
const uint32_t CPAL_I2C_TXDR[3] = {CPAL_I2C1_TXDR,CPAL_I2C2_TXDR,CPAL_I2C3_TXDR};
const uint32_t CPAL_I2C_RXDR[3] = {CPAL_I2C1_RXDR,CPAL_I2C2_RXDR,CPAL_I2C3_RXDR};

const GPIO_TypeDef* CPAL_I2C_SCL_GPIO_PORT[3] = {CPAL_I2C1_SCL_GPIO_PORT,CPAL_I2C2_SCL_GPIO_PORT,CPAL_I2C3_SCL_GPIO_PORT};
const uint16_t CPAL_I2C_SCL_GPIO_PIN[3] = {CPAL_I2C1_SCL_GPIO_PIN,CPAL_I2C2_SCL_GPIO_PIN,CPAL_I2C3_SCL_GPIO_PIN};
const uint32_t CPAL_I2C_SCL_GPIO_CLK[3] = {CPAL_I2C1_SCL_GPIO_CLK,CPAL_I2C2_SCL_GPIO_CLK,CPAL_I2C3_SCL_GPIO_CLK};
const uint16_t CPAL_I2C_SCL_GPIO_PINSOURCE[3] = {CPAL_I2C1_SCL_GPIO_PINSOURCE,CPAL_I2C2_SCL_GPIO_PINSOURCE,CPAL_I2C3_SCL_GPIO_PINSOURCE};
const uint16_t CPAL_I2C_SCL_GPIO_AF[3] = {CPAL_I2C1_SCL_GPIO_AF,CPAL_I2C2_SCL_GPIO_AF,CPAL_I2C3_SCL_GPIO_AF};

const GPIO_TypeDef* CPAL_I2C_SDA_GPIO_PORT[3] = {CPAL_I2C1_SDA_GPIO_PORT,CPAL_I2C2_SDA_GPIO_PORT,CPAL_I2C3_SDA_GPIO_PORT};
const uint16_t CPAL_I2C_SDA_GPIO_PIN[3] = {CPAL_I2C1_SDA_GPIO_PIN,CPAL_I2C2_SDA_GPIO_PIN,CPAL_I2C3_SDA_GPIO_PIN};
const uint32_t CPAL_I2C_SDA_GPIO_CLK[3] = {CPAL_I2C1_SDA_GPIO_CLK,CPAL_I2C2_SDA_GPIO_CLK,CPAL_I2C3_SDA_GPIO_CLK};
const uint16_t CPAL_I2C_SDA_GPIO_PINSOURCE[3] = {CPAL_I2C1_SDA_GPIO_PINSOURCE,CPAL_I2C2_SDA_GPIO_PINSOURCE,CPAL_I2C3_SDA_GPIO_PINSOURCE};
const uint16_t CPAL_I2C_SDA_GPIO_AF[3] = {CPAL_I2C1_SDA_GPIO_AF,CPAL_I2C2_SDA_GPIO_AF,CPAL_I2C3_SDA_GPIO_AF};

const uint32_t CPAL_I2C_DMA_CLK[3] = {CPAL_I2C1_DMA_CLK,CPAL_I2C2_DMA_CLK,CPAL_I2C3_DMA_CLK};

DMA_Channel_TypeDef* CPAL_I2C_DMA_TX_Channel[3] = {CPAL_I2C1_DMA_TX_Channel,CPAL_I2C2_DMA_TX_Channel,CPAL_I2C3_DMA_TX_Channel};
DMA_Channel_TypeDef* CPAL_I2C_DMA_RX_Channel[3] = {CPAL_I2C1_DMA_RX_Channel,CPAL_I2C2_DMA_RX_Channel,CPAL_I2C3_DMA_RX_Channel};

const IRQn_Type CPAL_I2C_DMA_TX_IRQn[3] = {CPAL_I2C1_DMA_TX_IRQn,CPAL_I2C2_DMA_TX_IRQn,CPAL_I2C3_DMA_TX_IRQn};
const IRQn_Type CPAL_I2C_DMA_RX_IRQn[3] = {CPAL_I2C1_DMA_RX_IRQn,CPAL_I2C2_DMA_RX_IRQn,CPAL_I2C3_DMA_RX_IRQn};

const IRQn_Type CPAL_I2C_IT_EVT_IRQn[3] = {CPAL_I2C1_IT_EVT_IRQn,CPAL_I2C2_IT_EVT_IRQn,CPAL_I2C3_IT_EVT_IRQn};
const IRQn_Type CPAL_I2C_IT_ERR_IRQn[3] = {CPAL_I2C1_IT_ERR_IRQn,CPAL_I2C2_IT_ERR_IRQn,CPAL_I2C3_IT_ERR_IRQn};

const uint8_t I2C_IT_EVT_PREPRIO[3] = {I2C1_IT_EVT_PREPRIO,I2C2_IT_EVT_PREPRIO,I2C3_IT_EVT_PREPRIO};
const uint8_t I2C_IT_EVT_SUBPRIO[3] = {I2C1_IT_EVT_SUBPRIO,I2C2_IT_EVT_SUBPRIO,I2C3_IT_EVT_SUBPRIO};

const uint8_t I2C_IT_ERR_PREPRIO[3] = {I2C1_IT_ERR_PREPRIO,I2C2_IT_ERR_PREPRIO,I2C3_IT_ERR_PREPRIO};
const uint8_t I2C_IT_ERR_SUBPRIO[3] = {I2C1_IT_ERR_SUBPRIO,I2C2_IT_ERR_SUBPRIO,I2C3_IT_ERR_SUBPRIO};

const uint8_t I2C_IT_DMATX_PREPRIO[3] = {I2C1_IT_DMATX_PREPRIO,I2C2_IT_DMATX_PREPRIO,I2C3_IT_DMATX_PREPRIO};
const uint8_t I2C_IT_DMATX_SUBPRIO[3] = {I2C1_IT_DMATX_SUBPRIO,I2C2_IT_DMATX_SUBPRIO,I2C3_IT_DMATX_SUBPRIO};

const uint8_t I2C_IT_DMARX_PREPRIO[3] = {I2C1_IT_DMARX_PREPRIO,I2C2_IT_DMARX_PREPRIO,I2C3_IT_DMARX_PREPRIO};
const uint8_t I2C_IT_DMARX_SUBPRIO[3] = {I2C1_IT_DMARX_SUBPRIO,I2C2_IT_DMARX_SUBPRIO,I2C3_IT_DMARX_SUBPRIO};

DMA_TypeDef* CPAL_I2C_DMA[3] = {CPAL_I2C1_DMA,CPAL_I2C2_DMA,CPAL_I2C3_DMA};

const uint32_t CPAL_I2C_DMA_TX_TC_FLAG[3] = {CPAL_I2C1_DMA_TX_TC_FLAG,CPAL_I2C2_DMA_TX_TC_FLAG,CPAL_I2C3_DMA_TX_TC_FLAG};
const uint32_t CPAL_I2C_DMA_TX_HT_FLAG[3] = {CPAL_I2C1_DMA_TX_HT_FLAG,CPAL_I2C2_DMA_TX_HT_FLAG,CPAL_I2C3_DMA_TX_HT_FLAG};
const uint32_t CPAL_I2C_DMA_TX_TE_FLAG[3] = {CPAL_I2C1_DMA_TX_TE_FLAG,CPAL_I2C3_DMA_TX_TE_FLAG,CPAL_I2C3_DMA_TX_TE_FLAG};

const uint32_t CPAL_I2C_DMA_RX_TC_FLAG[3] = {CPAL_I2C1_DMA_RX_TC_FLAG,CPAL_I2C2_DMA_RX_TC_FLAG,CPAL_I2C3_DMA_RX_TC_FLAG};
const uint32_t CPAL_I2C_DMA_RX_HT_FLAG[3] = {CPAL_I2C1_DMA_RX_HT_FLAG,CPAL_I2C2_DMA_RX_HT_FLAG,CPAL_I2C3_DMA_RX_HT_FLAG};
const uint32_t CPAL_I2C_DMA_RX_TE_FLAG[3] = {CPAL_I2C1_DMA_RX_TE_FLAG,CPAL_I2C2_DMA_RX_TE_FLAG,CPAL_I2C3_DMA_RX_TE_FLAG};

#elif defined (CPAL_I2C1_SUPPORTED) && defined (CPAL_I2C2_SUPPORTED)

I2C_TypeDef* CPAL_I2C_DEVICE[2] = {I2C1,I2C2};

const uint32_t CPAL_I2C_CLK[2] = {CPAL_I2C1_CLK,CPAL_I2C2_CLK};
const uint32_t CPAL_I2C_TXDR[2] = {CPAL_I2C1_TXDR,CPAL_I2C2_TXDR};
const uint32_t CPAL_I2C_RXDR[2] = {CPAL_I2C1_RXDR,CPAL_I2C2_RXDR};

const GPIO_TypeDef* CPAL_I2C_SCL_GPIO_PORT[2] = {CPAL_I2C1_SCL_GPIO_PORT,CPAL_I2C2_SCL_GPIO_PORT};
const uint16_t CPAL_I2C_SCL_GPIO_PIN[2] = {CPAL_I2C1_SCL_GPIO_PIN,CPAL_I2C2_SCL_GPIO_PIN};
const uint32_t CPAL_I2C_SCL_GPIO_CLK[2] = {CPAL_I2C1_SCL_GPIO_CLK,CPAL_I2C2_SCL_GPIO_CLK};
const uint16_t CPAL_I2C_SCL_GPIO_PINSOURCE[2] = {CPAL_I2C1_SCL_GPIO_PINSOURCE,CPAL_I2C2_SCL_GPIO_PINSOURCE};
const uint16_t CPAL_I2C_SCL_GPIO_AF[2] = {CPAL_I2C1_SCL_GPIO_AF,CPAL_I2C2_SCL_GPIO_AF};

const GPIO_TypeDef* CPAL_I2C_SDA_GPIO_PORT[2] = {CPAL_I2C1_SDA_GPIO_PORT,CPAL_I2C2_SDA_GPIO_PORT};
const uint16_t CPAL_I2C_SDA_GPIO_PIN[2] = {CPAL_I2C1_SDA_GPIO_PIN,CPAL_I2C2_SDA_GPIO_PIN};
const uint32_t CPAL_I2C_SDA_GPIO_CLK[2] = {CPAL_I2C1_SDA_GPIO_CLK,CPAL_I2C2_SDA_GPIO_CLK};
const uint16_t CPAL_I2C_SDA_GPIO_PINSOURCE[2] = {CPAL_I2C1_SDA_GPIO_PINSOURCE,CPAL_I2C2_SDA_GPIO_PINSOURCE};
const uint16_t CPAL_I2C_SDA_GPIO_AF[2] = {CPAL_I2C1_SDA_GPIO_AF,CPAL_I2C2_SDA_GPIO_AF};

const uint32_t CPAL_I2C_DMA_CLK[2] = {CPAL_I2C1_DMA_CLK,CPAL_I2C2_DMA_CLK};

DMA_Channel_TypeDef* CPAL_I2C_DMA_TX_Channel[2] = {CPAL_I2C1_DMA_TX_Channel,CPAL_I2C2_DMA_TX_Channel};
DMA_Channel_TypeDef* CPAL_I2C_DMA_RX_Channel[2] = {CPAL_I2C1_DMA_RX_Channel,CPAL_I2C2_DMA_RX_Channel};

const IRQn_Type CPAL_I2C_DMA_TX_IRQn[2] = {CPAL_I2C1_DMA_TX_IRQn,CPAL_I2C2_DMA_TX_IRQn};
const IRQn_Type CPAL_I2C_DMA_RX_IRQn[2] = {CPAL_I2C1_DMA_RX_IRQn,CPAL_I2C2_DMA_RX_IRQn};

const IRQn_Type CPAL_I2C_IT_EVT_IRQn[2] = {CPAL_I2C1_IT_EVT_IRQn,CPAL_I2C2_IT_EVT_IRQn};
const IRQn_Type CPAL_I2C_IT_ERR_IRQn[2] = {CPAL_I2C1_IT_ERR_IRQn,CPAL_I2C2_IT_ERR_IRQn};

const uint8_t I2C_IT_EVT_PREPRIO[2] = {I2C1_IT_EVT_PREPRIO,I2C2_IT_EVT_PREPRIO};
const uint8_t I2C_IT_EVT_SUBPRIO[2] = {I2C1_IT_EVT_SUBPRIO,I2C2_IT_EVT_SUBPRIO};

const uint8_t I2C_IT_ERR_PREPRIO[2] = {I2C1_IT_ERR_PREPRIO,I2C2_IT_ERR_PREPRIO};
const uint8_t I2C_IT_ERR_SUBPRIO[2] = {I2C1_IT_ERR_SUBPRIO,I2C2_IT_ERR_SUBPRIO};

const uint8_t I2C_IT_DMATX_PREPRIO[2] = {I2C1_IT_DMATX_PREPRIO,I2C2_IT_DMATX_PREPRIO};
const uint8_t I2C_IT_DMATX_SUBPRIO[2] = {I2C1_IT_DMATX_SUBPRIO,I2C2_IT_DMATX_SUBPRIO};

const uint8_t I2C_IT_DMARX_PREPRIO[2] = {I2C1_IT_DMARX_PREPRIO,I2C2_IT_DMARX_PREPRIO};
const uint8_t I2C_IT_DMARX_SUBPRIO[2] = {I2C1_IT_DMARX_SUBPRIO,I2C2_IT_DMARX_SUBPRIO};

DMA_TypeDef* CPAL_I2C_DMA[2] = {CPAL_I2C1_DMA,CPAL_I2C2_DMA};

const uint32_t CPAL_I2C_DMA_TX_TC_FLAG[2] = {CPAL_I2C1_DMA_TX_TC_FLAG,CPAL_I2C2_DMA_TX_TC_FLAG};
const uint32_t CPAL_I2C_DMA_TX_HT_FLAG[2] = {CPAL_I2C1_DMA_TX_HT_FLAG,CPAL_I2C2_DMA_TX_HT_FLAG};
const uint32_t CPAL_I2C_DMA_TX_TE_FLAG[2] = {CPAL_I2C1_DMA_TX_TE_FLAG,CPAL_I2C2_DMA_TX_TE_FLAG};

const uint32_t CPAL_I2C_DMA_RX_TC_FLAG[2] = {CPAL_I2C1_DMA_RX_TC_FLAG,CPAL_I2C2_DMA_RX_TC_FLAG};
const uint32_t CPAL_I2C_DMA_RX_HT_FLAG[2] = {CPAL_I2C1_DMA_RX_HT_FLAG,CPAL_I2C2_DMA_RX_HT_FLAG};
const uint32_t CPAL_I2C_DMA_RX_TE_FLAG[2] = {CPAL_I2C1_DMA_RX_TE_FLAG,CPAL_I2C2_DMA_RX_TE_FLAG};

#else

I2C_TypeDef* CPAL_I2C_DEVICE[1] = {I2C1};

const uint32_t CPAL_I2C_CLK[1] = {CPAL_I2C1_CLK};
const uint32_t CPAL_I2C_TXDR[1] = {CPAL_I2C1_TXDR};
const uint32_t CPAL_I2C_RXDR[1] = {CPAL_I2C1_RXDR};

const GPIO_TypeDef* CPAL_I2C_SCL_GPIO_PORT[1] = {CPAL_I2C1_SCL_GPIO_PORT};
const uint16_t CPAL_I2C_SCL_GPIO_PIN[1] = {CPAL_I2C1_SCL_GPIO_PIN};
const uint32_t CPAL_I2C_SCL_GPIO_CLK[1] = {CPAL_I2C1_SCL_GPIO_CLK};
const uint16_t CPAL_I2C_SCL_GPIO_PINSOURCE[1] = {CPAL_I2C1_SCL_GPIO_PINSOURCE};
const uint16_t CPAL_I2C_SCL_GPIO_AF[1] = {CPAL_I2C1_SCL_GPIO_AF};

const GPIO_TypeDef* CPAL_I2C_SDA_GPIO_PORT[1] = {CPAL_I2C1_SDA_GPIO_PORT};
const uint16_t CPAL_I2C_SDA_GPIO_PIN[1] = {CPAL_I2C1_SDA_GPIO_PIN};
const uint32_t CPAL_I2C_SDA_GPIO_CLK[1] = {CPAL_I2C1_SDA_GPIO_CLK};
const uint16_t CPAL_I2C_SDA_GPIO_PINSOURCE[1] = {CPAL_I2C1_SDA_GPIO_PINSOURCE};
const uint16_t CPAL_I2C_SDA_GPIO_AF[1] = {CPAL_I2C1_SDA_GPIO_AF};

const uint32_t CPAL_I2C_DMA_CLK[1] = {CPAL_I2C1_DMA_CLK};

DMA_Channel_TypeDef* CPAL_I2C_DMA_TX_Channel[1] = {CPAL_I2C1_DMA_TX_Channel};
DMA_Channel_TypeDef* CPAL_I2C_DMA_RX_Channel[1] = {CPAL_I2C1_DMA_RX_Channel};

const IRQn_Type CPAL_I2C_DMA_TX_IRQn[1] = {CPAL_I2C1_DMA_TX_IRQn};
const IRQn_Type CPAL_I2C_DMA_RX_IRQn[1] = {CPAL_I2C1_DMA_RX_IRQn};

const IRQn_Type CPAL_I2C_IT_EVT_IRQn[1] = {CPAL_I2C1_IT_EVT_IRQn};
const IRQn_Type CPAL_I2C_IT_ERR_IRQn[1] = {CPAL_I2C1_IT_ERR_IRQn};

const uint8_t I2C_IT_EVT_PREPRIO[1] = {I2C1_IT_EVT_PREPRIO};
const uint8_t I2C_IT_EVT_SUBPRIO[1] = {I2C1_IT_EVT_SUBPRIO};

const uint8_t I2C_IT_ERR_PREPRIO[1] = {I2C1_IT_ERR_PREPRIO};
const uint8_t I2C_IT_ERR_SUBPRIO[1] = {I2C1_IT_ERR_SUBPRIO};

const uint8_t I2C_IT_DMATX_PREPRIO[1] = {I2C1_IT_DMATX_PREPRIO};
const uint8_t I2C_IT_DMATX_SUBPRIO[1] = {I2C1_IT_DMATX_SUBPRIO};

const uint8_t I2C_IT_DMARX_PREPRIO[1] = {I2C1_IT_DMARX_PREPRIO};
const uint8_t I2C_IT_DMARX_SUBPRIO[1] = {I2C1_IT_DMARX_SUBPRIO};

DMA_TypeDef* CPAL_I2C_DMA[1] = {CPAL_I2C1_DMA};

const uint32_t CPAL_I2C_DMA_TX_TC_FLAG[1] = {CPAL_I2C1_DMA_TX_TC_FLAG};
const uint32_t CPAL_I2C_DMA_TX_HT_FLAG[1] = {CPAL_I2C1_DMA_TX_HT_FLAG};
const uint32_t CPAL_I2C_DMA_TX_TE_FLAG[1] = {CPAL_I2C1_DMA_TX_TE_FLAG};

const uint32_t CPAL_I2C_DMA_RX_TC_FLAG[1] = {CPAL_I2C1_DMA_RX_TC_FLAG};
const uint32_t CPAL_I2C_DMA_RX_HT_FLAG[1] = {CPAL_I2C1_DMA_RX_HT_FLAG};
const uint32_t CPAL_I2C_DMA_RX_TE_FLAG[1] = {CPAL_I2C1_DMA_RX_TE_FLAG};
#endif /* CPAL_I2C1_SUPPORTED && CPAL_I2C2_SUPPORTED && CPAL_I2C3_SUPPORTED */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*================== CPAL_I2C_HAL_Config ==================*/

/**
  * @brief  Configure NVIC Priority Group.
  * @param  None.
  * @retval None.
  */
void CPAL_HAL_NVICInit(void)
{
 /* Set NVIC Group Priority */
  NVIC_PriorityGroupConfig (CPAL_NVIC_PRIOGROUP);
}

/**
  * @brief  Reset then enable the I2C device clock.
  * @param  Device : I2C Device instance.
  * @retval None
  */
void CPAL_I2C_HAL_CLKInit(CPAL_DevTypeDef Device)
{
  /* Reset I2Cx device clock in order to avoid non-cleared error flags */
  __I2C_RCC_RESET(CPAL_I2C_CLK [Device]);

  /* Enable I2Cx device clock */
  __I2C_CLK_CMD(CPAL_I2C_CLK [Device], ENABLE);
}


/**
  * @brief  Reset then disable the I2C device clock.
  * @param  Device : I2C Device instance
  * @retval None.
  */
void CPAL_I2C_HAL_CLKDeInit(CPAL_DevTypeDef Device)
{
  /* Reset I2Cx device clock in order to avoid non-cleared error flags */
  __I2C_RCC_RESET(CPAL_I2C_CLK[Device]);

  /* Disable I2Cx device clock */
  __I2C_CLK_CMD(CPAL_I2C_CLK[Device], DISABLE);
}


/**
  * @brief  Configure the IO pins used by the I2C device.
  * @param  Device : I2C Device instance.
  * @retval None.
  */
void CPAL_I2C_HAL_GPIOInit(CPAL_DevTypeDef Device)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable I2Cx SCL and SDA Pin Clock */
  __I2C_GPIO_CLK_CMD((CPAL_I2C_SCL_GPIO_CLK[Device] | CPAL_I2C_SDA_GPIO_CLK[Device]), ENABLE);

  /* Connect PXx to I2C_SCL */
  GPIO_PinAFConfig((GPIO_TypeDef*)CPAL_I2C_SCL_GPIO_PORT[Device],CPAL_I2C_SCL_GPIO_PINSOURCE[Device],CPAL_I2C_SCL_GPIO_AF[Device]);

  /* Connect PXx to I2C_SDA */
  GPIO_PinAFConfig((GPIO_TypeDef*)CPAL_I2C_SDA_GPIO_PORT[Device],CPAL_I2C_SDA_GPIO_PINSOURCE[Device],CPAL_I2C_SDA_GPIO_AF[Device]);

  /* Set GPIO frequency to 50MHz */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  /* Select Alternate function mode */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  /* Select output Open Drain type */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;

  /* Disable internal Pull-up */
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  /* Initialize I2Cx SCL Pin */
  GPIO_InitStructure.GPIO_Pin = CPAL_I2C_SCL_GPIO_PIN[Device];
  GPIO_Init((GPIO_TypeDef*)CPAL_I2C_SCL_GPIO_PORT[Device], &GPIO_InitStructure);

  /* Initialize I2Cx SDA Pin */
  GPIO_InitStructure.GPIO_Pin = CPAL_I2C_SDA_GPIO_PIN[Device];
  GPIO_Init((GPIO_TypeDef*)CPAL_I2C_SDA_GPIO_PORT[Device], &GPIO_InitStructure);
}


/**
  * @brief  Deinitialize the IO pins used by the I2C device
  *         (configured to their default state).
  * @param  Device : I2C Device instance.
  * @retval None.
  */
void CPAL_I2C_HAL_GPIODeInit(CPAL_DevTypeDef Device)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Set GPIO frequency to 50MHz */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  /* Select Input floating mode */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

  /* Select output Open Drain type */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;

  /* Disable internal Pull-up */
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  /* Deinitialize I2Cx SCL Pin */
  GPIO_InitStructure.GPIO_Pin = CPAL_I2C_SCL_GPIO_PIN[Device];
  GPIO_Init((GPIO_TypeDef*)CPAL_I2C_SCL_GPIO_PORT[Device], &GPIO_InitStructure);

  /* Deinitialize I2Cx SDA Pin */
  GPIO_InitStructure.GPIO_Pin = CPAL_I2C_SDA_GPIO_PIN[Device];
  GPIO_Init((GPIO_TypeDef*)CPAL_I2C_SDA_GPIO_PORT[Device], &GPIO_InitStructure);
}


#ifdef CPAL_I2C_DMA_PROGMODEL
/**
  * @brief  Enable the DMA clock and initialize needed DMA Channels
  *         used by the I2C device.
  * @param  Device : I2C Device instance.
  * @param  Direction : Transfer direction.
  * @param  Options : Transfer Options.
  * @retval None.
  */
void CPAL_I2C_HAL_DMAInit(CPAL_DevTypeDef Device, CPAL_DirectionTypeDef Direction, uint32_t Options)
{
  /* Enable I2Cx DMA */
  __DMA_CLK_CMD(CPAL_I2C_DMA_CLK[Device], ENABLE);

  /* I2Cx Common Channel Configuration */
  CPAL_DMA_InitStructure.DMA_BufferSize = 0xFFFF;
  CPAL_DMA_InitStructure.DMA_PeripheralInc =  DMA_PeripheralInc_Disable;
  CPAL_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  CPAL_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte ;
  CPAL_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  CPAL_DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  CPAL_DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  CPAL_DMA_InitStructure.DMA_MemoryBaseAddr = 0;
  CPAL_DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

  /* If TX Direction (Transmission) selected */
  if ((Direction & CPAL_DIRECTION_TX) != 0)
  {
    /* Select I2Cx TXDR Address register as DMA PeripheralBaseAddress */
    CPAL_DMA_InitStructure.DMA_PeripheralBaseAddr = CPAL_I2C_TXDR [Device];

    /* Select Memory to Peripheral transfer direction */
    CPAL_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;

    /* Initialize I2Cx DMA Tx Channel */
    DMA_Init((DMA_Channel_TypeDef*)CPAL_I2C_DMA_TX_Channel[Device], &CPAL_DMA_InitStructure);
  }

  /* If RX Direction (Reception) selected */
  if ((Direction & CPAL_DIRECTION_RX ) != 0)
  {
    /* Select I2Cx RXDR Address register as DMA PeripheralBaseAddress */
    CPAL_DMA_InitStructure.DMA_PeripheralBaseAddr = CPAL_I2C_RXDR [Device];

    /* Select Peripheral to Memory transfer direction */
    CPAL_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

    /* Initialize I2Cx DMA Rx Channel */
    DMA_Init((DMA_Channel_TypeDef*)CPAL_I2C_DMA_RX_Channel[Device], &CPAL_DMA_InitStructure);
  }
}


/**
  * @brief  Configure the DMA channel specific for TX transfer.
  * @param  Device : I2C Device instance.
  * @param  TXferStruct : DMA TX Transfer Parameters.
  * @param  Options :  Transfer Options.
  * @retval None.
  */
void CPAL_I2C_HAL_DMATXConfig(CPAL_DevTypeDef Device, CPAL_TransferTypeDef* TxXferStruct, uint32_t Options )
{
  /* Set Memory Base Address */
  CPAL_DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(TxXferStruct->pbBuffer);

  /* Set number of data */
  CPAL_DMA_InitStructure.DMA_BufferSize = TxXferStruct->wNumData;

  /* Select I2Cx TXDR Address register as DMA PeripheralBaseAddress */
  CPAL_DMA_InitStructure.DMA_PeripheralBaseAddr = CPAL_I2C_TXDR [Device];

  /* If TX DMA Circular Mode Option Bit Selected */
  if ((Options & CPAL_OPT_DMATX_CIRCULAR) != 0)
  {
    /* Select DMA Circular Mode */
    CPAL_DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  }
  /* If TX DMA Circular Mode Option Bit not selected */
  else
  {
    /* Select DMA Normal Mode */
    CPAL_DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  }

  /* Select Peripheral to Memory transfer direction */
  CPAL_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;

  /* Initialize I2Cx DMA Tx Channel */
  DMA_Init((DMA_Channel_TypeDef*)CPAL_I2C_DMA_TX_Channel[Device], &CPAL_DMA_InitStructure);
}


/**
  * @brief  Configure the DMA channel specific for RX transfer.
  * @param  Device : I2C Device instance.
  * @param  RXferStruct : DMA RX Transfer Parameters.
  * @param  Options :  Transfer Options.
  * @retval None.
  */
void CPAL_I2C_HAL_DMARXConfig(CPAL_DevTypeDef Device, CPAL_TransferTypeDef* RxXferStruct, uint32_t Options )
{
  /* Set Memory Base Address */
  CPAL_DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(RxXferStruct->pbBuffer);

  /* Set number of data */
  CPAL_DMA_InitStructure.DMA_BufferSize = RxXferStruct->wNumData;

  /* Select I2Cx RXDR Address register as DMA PeripheralBaseAddress */
  CPAL_DMA_InitStructure.DMA_PeripheralBaseAddr = CPAL_I2C_RXDR [Device];

  /* If RX DMA Circular Mode Option Bit Selected */
  if ((Options & CPAL_OPT_DMARX_CIRCULAR) != 0)
  {
    /* Select DMA Circular Mode */
    CPAL_DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  }
  /* If RX DMA Circular Mode Option Bit not selected */
  else
  {
    /* Select DMA Normal Mode */
    CPAL_DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  }

  /* Select Peripheral to Memory transfer direction */
  CPAL_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

  /* Initialize I2Cx DMA Rx Channel */
  DMA_Init((DMA_Channel_TypeDef*)CPAL_I2C_DMA_RX_Channel[Device], &CPAL_DMA_InitStructure);
}

/**
  * @brief  Deinitialize the DMA channel used by I2C Device(configured to their default state).
  *         DMA clock is not disabled.
  * @param  Device : I2C Device instance.
  * @param  Direction : Transfer direction.
  * @retval None.
  */
void CPAL_I2C_HAL_DMADeInit(CPAL_DevTypeDef Device, CPAL_DirectionTypeDef Direction)
{
  /* If TX Direction (Transmission) selected */
  if ((Direction & CPAL_DIRECTION_TX) != 0)
  {
    /* Deinitialize I2Cx DMA Tx Channel */
    DMA_DeInit((DMA_Channel_TypeDef*)CPAL_I2C_DMA_TX_Channel[Device]);
  }

  /* If RX Direction (Reception) selected */
  if ((Direction & CPAL_DIRECTION_RX) != 0)
  {
    /* Deinitialize I2Cx DMA Rx Channel */
    DMA_DeInit((DMA_Channel_TypeDef*)CPAL_I2C_DMA_RX_Channel[Device]);
  }
}
#endif /* CPAL_I2C_DMA_PROGMODEL */


/**
  * @brief  Configure NVIC and interrupts used by I2C Device according to
  *         enabled options
  * @param  Device : I2C Device instance.
  * @param  Options : I2C Transfer Options.
  * @param  Direction : I2C Transfer direction.
  * @param  ProgModel : I2C Transfer Progmodel.
  * @retval None.
  */
void CPAL_I2C_HAL_ITInit(CPAL_DevTypeDef Device, uint32_t Options, CPAL_DirectionTypeDef Direction, CPAL_ProgModelTypeDef ProgModel)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure NVIC priority Group */
  CPAL_HAL_NVICInit();

  /* Enable the IRQ channel */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  /* Configure NVIC for I2Cx EVT Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = CPAL_I2C_IT_EVT_IRQn[Device] ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = I2C_IT_EVT_PREPRIO[Device];
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = I2C_IT_EVT_SUBPRIO[Device];
  NVIC_Init(&NVIC_InitStructure);

  /* If I2C ERR Interrupt Option Bit not selected */
  if ((Options & CPAL_OPT_I2C_ERRIT_DISABLE) == 0)
  {
    /* Configure NVIC for I2Cx ERR Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = CPAL_I2C_IT_ERR_IRQn[Device] ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = I2C_IT_ERR_PREPRIO[Device];
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = I2C_IT_ERR_SUBPRIO[Device];
    NVIC_Init(&NVIC_InitStructure);

    /* Enable I2C Error Interrupts */
    __CPAL_I2C_HAL_ENABLE_ERRIT(Device);
  }

#ifdef CPAL_I2C_DMA_PROGMODEL
  if (ProgModel == CPAL_PROGMODEL_DMA)
  {
    if ((Direction & CPAL_DIRECTION_TX) != 0)
    {
      /* Configure NVIC for DMA TX channel interrupt */
      NVIC_InitStructure.NVIC_IRQChannel = CPAL_I2C_DMA_TX_IRQn [Device] ;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = I2C_IT_DMATX_PREPRIO[Device];
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = I2C_IT_DMATX_SUBPRIO[Device];
      NVIC_Init(&NVIC_InitStructure);

      /* Enable DMA TX Channel TCIT  */
      __I2C_HAL_ENABLE_DMATX_TCIT(Device);

      /* Enable DMA TX Channel TEIT  */
      __I2C_HAL_ENABLE_DMATX_TEIT(Device);

      /* If DMA TX HT interrupt Option Bits Selected */
      if ((Options & CPAL_OPT_DMATX_HTIT) != 0)
      {
        /* Enable DMA TX Channel HTIT  */
        __I2C_HAL_ENABLE_DMATX_HTIT(Device);
      }
    }

    if ((Direction & CPAL_DIRECTION_RX) != 0)
    {
      /* Configure NVIC for DMA RX channel interrupt */
      NVIC_InitStructure.NVIC_IRQChannel = CPAL_I2C_DMA_RX_IRQn [Device] ;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = I2C_IT_DMARX_PREPRIO[Device];
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = I2C_IT_DMARX_SUBPRIO[Device];
      NVIC_Init(&NVIC_InitStructure);

      /* Enable DMA RX Channel TCIT  */
      __I2C_HAL_ENABLE_DMARX_TCIT(Device);

      /* Enable DMA RX Channel TEIT  */
      __I2C_HAL_ENABLE_DMARX_TEIT(Device);

      /* If DMA RX HT interrupt Option Bits Selected */
      if ((Options & CPAL_OPT_DMARX_HTIT) != 0)
      {
        /* Enable DMA RX Channel HTIT  */
        __I2C_HAL_ENABLE_DMARX_HTIT(Device);
      }
    }
  }
#endif /* CPAL_I2C_DMA_PROGMODEL */
}


/**
  * @brief  Deinitialize NVIC and interrupts used by I2C Device in
  *         the current Configuration.
  * @param  Device : I2C Device instance.
  * @param  Options : I2C Transfer Options.
  * @param  Direction : I2C Transfer direction.
  * @param  ProgModel : I2C Transfer Progmodel.
  * @retval None.
  */
void CPAL_I2C_HAL_ITDeInit(CPAL_DevTypeDef Device, uint32_t Options, CPAL_DirectionTypeDef Direction, CPAL_ProgModelTypeDef ProgModel)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

  /* Disable the IRQ channel */
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;

  /* Disable I2Cx EVT IRQn */
  NVIC_InitStructure.NVIC_IRQChannel = CPAL_I2C_IT_EVT_IRQn[Device];
  NVIC_Init(&NVIC_InitStructure);

  /* If I2C ERR Interrupt Option Bit Deselected */
  if ((Options & CPAL_OPT_I2C_ERRIT_DISABLE) == 0)
  {
    /* Disable I2Cx ERR IRQn */
    NVIC_InitStructure.NVIC_IRQChannel = CPAL_I2C_IT_ERR_IRQn[Device];
    NVIC_Init(&NVIC_InitStructure);
  }

#ifdef CPAL_I2C_DMA_PROGMODEL
  if (ProgModel == CPAL_PROGMODEL_DMA)
  {
    if ((Direction & CPAL_DIRECTION_TX) != 0)
    {
      /* Disable I2Cx DMA TX IRQn */
      NVIC_InitStructure.NVIC_IRQChannel = CPAL_I2C_DMA_TX_IRQn [Device] ;
      NVIC_Init(&NVIC_InitStructure);
    }

    if ((Direction & CPAL_DIRECTION_RX) != 0)
    {
      /* Disable I2Cx DMA RX IRQn */
      NVIC_InitStructure.NVIC_IRQChannel = CPAL_I2C_DMA_RX_IRQn [Device] ;
      NVIC_Init(&NVIC_InitStructure);
    }
  }
#endif /* CPAL_I2C_DMA_PROGMODEL */
}


/*================== CPAL_I2C1_IRQhandler ==================*/
#ifdef CPAL_I2C1_SUPPORTED
  #ifdef CPAL_USE_I2C1

/**
  * @brief  This function handles I2C1 interrupt request.
  * @param  None.
  * @retval CPAL_PASS.
  */
uint32_t I2C1_EV_IRQHandler(void)
{
 /* Call the Common Event handler function */
 return CPAL_I2C_EV_IRQHandler(&I2C1_DevStructure);
}


/**
  * @brief  This function handles I2C1 Errors interrupt.
  * @param  None.
  * @retval CPAL_PASS.
  */
uint32_t I2C1_ER_IRQHandler(void)
{
  CPAL_LOG("\n\r\n\rLOG <I2C1_ER_IRQHandler> : I2C1 Device Error IT ");

  /* Call the Common Error handler function */
  CPAL_I2C_ER_IRQHandler(&I2C1_DevStructure);

  return CPAL_PASS;
}

  #ifdef CPAL_I2C_DMA_PROGMODEL
/**
  * @brief  This function handles I2C1 TX DMA interrupt request.
  * @param  None.
  * @retval CPAL_PASS.
  */
uint32_t CPAL_I2C1_DMA_TX_IRQHandler(void)
{
  /* Call the Common DMA TX handler function */
  return CPAL_I2C_DMA_TX_IRQHandler(&I2C1_DevStructure);
}


/**
  * @brief  This function handles I2C1 RX DMA interrupt request.
  * @param  None.
  * @retval CPAL_PASS.
  */
uint32_t CPAL_I2C1_DMA_RX_IRQHandler(void)
{
  /* Call the Common DMA RX handler function */
  return CPAL_I2C_DMA_RX_IRQHandler(&I2C1_DevStructure);
}
   #endif /* CPAL_I2C_DMA_PROGMODEL */
  #endif /* CPAL_USE_I2C1 */
#endif /* CPAL_I2C1_SUPPORTED */


/*================== CPAL_I2C2_IRQhandler ==================*/
#ifdef CPAL_I2C2_SUPPORTED
  #ifdef CPAL_USE_I2C2

/**
  * @brief  This function handles I2C2 interrupt request.
  * @param  None.
  * @retval CPAL_PASS.
  */
uint32_t I2C2_EV_IRQHandler(void)
{
  /* Call the Common Event handler function */
  return CPAL_I2C_EV_IRQHandler(&I2C2_DevStructure);
}


/**
  * @brief  This function handles I2C2 Errors interrupt.
  * @param  None.
  * @retval CPAL_PASS.
  */
uint32_t I2C2_ER_IRQHandler(void)
{
  CPAL_LOG("\n\r\n\rLOG <I2C2_ER_IRQHandler> : I2C2 Device Error IT ");

  /* Call the Common Error handler function */
  CPAL_I2C_ER_IRQHandler(&I2C2_DevStructure);

  return CPAL_PASS;
}


 #ifdef CPAL_I2C_DMA_PROGMODEL
/**
  * @brief  This function handles I2C2 TX DMA interrupt request.
  * @param  None.
  * @retval CPAL_PASS.
  */
uint32_t CPAL_I2C2_DMA_TX_IRQHandler(void)
{
  /* Call the Common DMA TX handler function */
  return CPAL_I2C_DMA_TX_IRQHandler(&I2C2_DevStructure);
}


/**
  * @brief  This function handles I2C2 RX DMA interrupt request.
  * @param  None.
  * @retval CPAL_PASS.
  */
uint32_t CPAL_I2C2_DMA_RX_IRQHandler(void)
{
  /* Call the Common DMA RX handler function */
  return CPAL_I2C_DMA_RX_IRQHandler(&I2C2_DevStructure);
}
   #endif /* CPAL_I2C_DMA_PROGMODEL */
  #endif /* CPAL_USE_I2C2 */
#endif /* CPAL_I2C2_SUPPORTED */

/*================== CPAL_I2C3_IRQhandler ==================*/
#ifdef CPAL_I2C3_SUPPORTED
  #ifdef CPAL_USE_I2C3

/**
  * @brief  This function handles I2C3 interrupt request.
  * @param  None.
  * @retval CPAL_PASS.
  */
uint32_t I2C3_EV_IRQHandler(void)
{
  /* Call the Common Event handler function */
  return CPAL_I2C_EV_IRQHandler(&I2C3_DevStructure);
}


/**
  * @brief  This function handles I2C3 Errors interrupt.
  * @param  None.
  * @retval CPAL_PASS.
  */
uint32_t I2C3_ER_IRQHandler(void)
{
  CPAL_LOG("\n\r\n\rLOG <I2C3_ER_IRQHandler> : I2C3 Device Error IT ");

  /* Call the Common Error handler function */
  CPAL_I2C_ER_IRQHandler(&I2C3_DevStructure);

  return CPAL_PASS;
}


 #ifdef CPAL_I2C_DMA_PROGMODEL
/**
  * @brief  This function handles I2C3 TX DMA interrupt request.
  * @param  None.
  * @retval CPAL_PASS.
  */
uint32_t CPAL_I2C3_DMA_TX_IRQHandler(void)
{
  /* Call the Common DMA TX handler function */
  return CPAL_I2C_DMA_TX_IRQHandler(&I2C3_DevStructure);
}


/**
  * @brief  This function handles I2C3 RX DMA interrupt request.
  * @param  None.
  * @retval CPAL_PASS.
  */
uint32_t CPAL_I2C3_DMA_RX_IRQHandler(void)
{
  /* Call the Common DMA RX handler function */
  return CPAL_I2C_DMA_RX_IRQHandler(&I2C3_DevStructure);
}
   #endif /* CPAL_I2C_DMA_PROGMODEL */
  #endif /* CPAL_USE_I2C3 */
#endif /* CPAL_I2C1_SUPPORTED */

