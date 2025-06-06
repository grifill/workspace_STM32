/**
  ******************************************************************************
  * @file    SPI/SPI_TwoBoards/SPI_DataExchangeInterrupt/main.h
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

/* Exported typedef ----------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Exported define -----------------------------------------------------------*/

/* Uncomment the line below if you will use the SPI peripheral as a Master */
/* #define SPI_MASTER */
/* Uncomment the line below if you will use the SPI peripheral as a Slave */
#define SPI_SLAVE

/* Uncomment the size of data to be transmitted (only one data size must be selected) */
/* #define SPI_DATASIZE_8 */
/* #define SPI_DATASIZE_7 */
/* #define SPI_DATASIZE_6 */
#define SPI_DATASIZE_5


/* USER_TIMEOUT value for waiting loops. This timeout is just guarantee that the
   application will not remain stuck if the USART communication is corrupted. 
   You may modify this timeout value depending on CPU frequency and application
   conditions (interrupts routines, number of data to transfer, baudrate, CPU
   frequency...). */ 
#define USER_TIMEOUT                    ((uint32_t)0x64) /* Waiting 1s */

/* Communication boards SPIx Interface */
#define SPIx                             SPI3
#define SPIx_CLK                         RCC_APB1Periph_SPI3
#define SPIx_IRQn                        SPI3_IRQn

#define SPIx_SCK_PIN                     GPIO_Pin_10
#define SPIx_SCK_GPIO_PORT               GPIOC
#define SPIx_SCK_GPIO_CLK                RCC_AHBPeriph_GPIOC
#define SPIx_SCK_SOURCE                  GPIO_PinSource10
#define SPIx_SCK_AF                      GPIO_AF_6

#define SPIx_MISO_PIN                    GPIO_Pin_11
#define SPIx_MISO_GPIO_PORT              GPIOC
#define SPIx_MISO_GPIO_CLK               RCC_AHBPeriph_GPIOC
#define SPIx_MISO_SOURCE                 GPIO_PinSource11
#define SPIx_MISO_AF                     GPIO_AF_6

#define SPIx_MOSI_PIN                    GPIO_Pin_12
#define SPIx_MOSI_GPIO_PORT              GPIOC
#define SPIx_MOSI_GPIO_CLK               RCC_AHBPeriph_GPIOC
#define SPIx_MOSI_SOURCE                 GPIO_PinSource12
#define SPIx_MOSI_AF                     GPIO_AF_6

#define TXBUFFERSIZE                     (countof(TxBuffer) - 1)
#define RXBUFFERSIZE                     TXBUFFERSIZE

/* Joystick Commands */
#define CMD_RIGHT                        0x15
#define CMD_LEFT                         0x1A
#define CMD_UP                           0x13
#define CMD_DOWN                         0x1C

#define CMD_ACK                          0x16 

/* Define numbers of bytes to transmit from TxBuffer */
#define DATA_SIZE                        TXBUFFERSIZE

/* Define data size and data masks */
#ifdef SPI_DATASIZE_8
 #define SPI_DATASIZE                     SPI_DataSize_8b
 #define SPI_DATAMASK                     (uint8_t)0xFF
#elif defined (SPI_DATASIZE_7)
 #define SPI_DATASIZE                     SPI_DataSize_7b
 #define SPI_DATAMASK                     (uint8_t)0x7F
#elif defined (SPI_DATASIZE_6)
 #define SPI_DATASIZE                     SPI_DataSize_6b
 #define SPI_DATAMASK                     (uint8_t)0x3F
#elif defined (SPI_DATASIZE_5)
 #define SPI_DATASIZE                     SPI_DataSize_5b
 #define SPI_DATAMASK                     (uint8_t)0x1F
#else
 #error " Select at least one data size in main.h file " 
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t GetVar_NbrOfData(void);

#endif /* __MAIN_H */


