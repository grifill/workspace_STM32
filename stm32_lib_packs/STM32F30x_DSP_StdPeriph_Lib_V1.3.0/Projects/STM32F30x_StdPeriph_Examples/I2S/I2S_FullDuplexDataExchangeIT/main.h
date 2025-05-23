/**
  ******************************************************************************
  * @file    I2S/I2S_FullDuplexDataExchangeIT/main.h 
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    14-December-2021
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "stm32303c_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported define ------------------------------------------------------------*/
/* Uncomment the line below if you will use the I2S peripheral as a Master */
/* #define I2S_MASTER_FULL_DUPLEX  */
/* Uncomment the line below if you will use the I2S peripheral as a Slave */
#define I2S_SLAVE_FULL_DUPLEX 

/* I2S Interface */
#define  SPI                        SPI3
#define  I2Sext                     I2S3ext
#define  RCC_APBPeriph_SPI          RCC_APB1Periph_SPI3
#define  RCC_AHBPeriph_GPIO        (RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOC)

#define I2S_GPIO_PORT               GPIOC 
#define I2S_GPIO_AF                 GPIO_AF_6

#define I2S_CK_PIN                  GPIO_Pin_10                  /* PC.10 */             
#define I2S_CK_SOURCE               GPIO_PinSource10

#define I2S_DIN_PIN                 GPIO_Pin_11                  /* PC.11 */              
#define I2S_DIN_SOURCE              GPIO_PinSource11

#define I2S_DOUT_PIN                GPIO_Pin_12                  /* PC.12 */              
#define I2S_DOUT_SOURCE             GPIO_PinSource12

#define I2S_WS_PIN                  GPIO_Pin_4                   /* PA.4 */              
#define I2S_WS_SOURCE               GPIO_PinSource4
#define I2S_WS_GPIO_PORT            GPIOA 
#define SPI_IRQHandler              SPI3_IRQHandler
#define SPI_IRQn                    SPI3_IRQn
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

