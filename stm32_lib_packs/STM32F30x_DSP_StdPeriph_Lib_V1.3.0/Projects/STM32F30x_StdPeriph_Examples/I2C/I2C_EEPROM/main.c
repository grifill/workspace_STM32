/**
  ******************************************************************************
  * @file    I2C/I2C_EEPROM/main.c 
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

/** @addtogroup I2C_EEPROM
  * @{
  */
  
/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
/* Uncomment the following line to enable using LCD screen for messages display */
#define ENABLE_LCD_MSG_DISPLAY

#define sEE_WRITE_ADDRESS1        0x50
#define sEE_READ_ADDRESS1         0x50
#define BUFFER_SIZE1             (countof(Tx1Buffer)-1)
#define BUFFER_SIZE2             (countof(Tx2Buffer)-1)
#define sEE_WRITE_ADDRESS2       (sEE_WRITE_ADDRESS1 + BUFFER_SIZE1)
#define sEE_READ_ADDRESS2        (sEE_READ_ADDRESS1 + BUFFER_SIZE1)

/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
uint8_t Tx1Buffer[] = "/* STM32F30x I2C Firmware Library EEPROM driver example: \
                        This firmware provides a basic example of how to use the I2C firmware library and\
                        an associate I2C EEPROM driver to communicate with an I2C EEPROM device \
                        I2C peripheral is configured in Master transmitter during write operation and in\
                        Master receiver during read operation from I2C EEPROM.*/";

uint8_t Tx2Buffer[] = "/* STM32F30x I2C Firmware Library EEPROM driver example : \
                        I2C2 interfacing with M24M01 EEPROM */";

uint8_t Rx1Buffer[BUFFER_SIZE1], Rx2Buffer[BUFFER_SIZE2]; 
__IO TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;
__IO uint32_t NumDataRead = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);

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

#ifdef ENABLE_LCD_MSG_DISPLAY
  /* Initialize the LCD screen for information display */
  STM32303C_LCD_Init();
  
  LCD_Clear(LCD_COLOR_BLUE);  
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_0, (uint8_t*) "STM32F30x FW Library");
  LCD_DisplayStringLine(LCD_LINE_1, (uint8_t*) "   EEPROM Example   ");
#endif /* ENABLE_LCD_MSG_DISPLAY */  
  
  /* Initialize the I2C EEPROM driver ----------------------------------------*/
  sEE_Init();  

  /* First write in the memory followed by a read of the written data --------*/
  /* Write on I2C EEPROM from sEE_WRITE_ADDRESS1 */
  sEE_WriteBuffer(Tx1Buffer, sEE_WRITE_ADDRESS1, BUFFER_SIZE1); 

  /* Wait for EEPROM standby state */
  sEE_WaitEepromStandbyState();  
  
  /* Set the Number of data to be read */
  NumDataRead = BUFFER_SIZE1;
  
  /* Read from I2C EEPROM from sEE_READ_ADDRESS1 */
  sEE_ReadBuffer(Rx1Buffer, sEE_READ_ADDRESS1, (uint16_t *)(&NumDataRead)); 
 
#ifdef ENABLE_LCD_MSG_DISPLAY  
  LCD_DisplayStringLine(LCD_LINE_3, (uint8_t*) " Transfer 1 Ongoing ");
#endif /* ENABLE_LCD_MSG_DISPLAY */ 
  
  /* Check if the data written to the memory is read correctly */
  TransferStatus1 = Buffercmp(Tx1Buffer, Rx1Buffer, BUFFER_SIZE1);
  /* TransferStatus1 = PASSED, if the transmitted and received data 
     to/from the EEPROM are the same */
  /* TransferStatus1 = FAILED, if the transmitted and received data 
     to/from the EEPROM are different */
#ifdef ENABLE_LCD_MSG_DISPLAY  
  if (TransferStatus1 == PASSED)
  {
    LCD_DisplayStringLine(LCD_LINE_3, (uint8_t*) " Transfer 1 PASSED  ");
  }
  else
  {
    LCD_DisplayStringLine(LCD_LINE_3, (uint8_t*) " Transfer 1 FAILED  ");
  }  
#endif /* ENABLE_LCD_MSG_DISPLAY */  

  /* Second write in the memory followed by a read of the written data -------*/
  /* Write on I2C EEPROM from sEE_WRITE_ADDRESS2 */
  sEE_WriteBuffer(Tx2Buffer, sEE_WRITE_ADDRESS2, BUFFER_SIZE2); 

  /* Wait for EEPROM standby state */
  sEE_WaitEepromStandbyState();  
  
  /* Set the Number of data to be read */
  NumDataRead = BUFFER_SIZE2;  
  
  /* Read from I2C EEPROM from sEE_READ_ADDRESS2 */
  sEE_ReadBuffer(Rx2Buffer, sEE_READ_ADDRESS2, (uint16_t *)(&NumDataRead));

#ifdef ENABLE_LCD_MSG_DISPLAY   
  LCD_DisplayStringLine(LCD_LINE_5, (uint8_t*) " Transfer 2 Ongoing ");
#endif /* ENABLE_LCD_MSG_DISPLAY */  
  
  /* Check if the data written to the memory is read correctly */
  TransferStatus2 = Buffercmp(Tx2Buffer, Rx2Buffer, BUFFER_SIZE2);
  /* TransferStatus2 = PASSED, if the transmitted and received data 
     to/from the EEPROM are the same */
  /* TransferStatus2 = FAILED, if the transmitted and received data 
     to/from the EEPROM are different */
#ifdef ENABLE_LCD_MSG_DISPLAY   
  if (TransferStatus2 == PASSED)
  {
    LCD_DisplayStringLine(LCD_LINE_5, (uint8_t*) " Transfer 2 PASSED  ");
  }
  else
  {
    LCD_DisplayStringLine(LCD_LINE_5, (uint8_t*) " Transfer 2 FAILED  ");
  }  
#endif /* ENABLE_LCD_MSG_DISPLAY */
  
  /* Free all used resources */
  sEE_DeInit();

  while (1)
  {
  }
}

#ifndef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Example of timeout situation management.
  * @param  None.
  * @retval None.
  */
uint32_t sEE_TIMEOUT_UserCallback(void)
{
  /* Use application may try to recover the communication by resetting I2C
    peripheral (calling the function I2C_SoftwareResetCmd()) then re-start
    the transmission/reception from a previously stored recover point.
    For simplicity reasons, this example only shows a basic way for errors 
    managements which consists of stopping all the process and requiring system
    reset. */
  
#ifdef ENABLE_LCD_MSG_DISPLAY   
  /* Display error message on screen */
  LCD_Clear(LCD_COLOR_RED);  
  LCD_DisplayStringLine(LCD_LINE_4, (uint8_t*) "Communication ERROR!");
  LCD_DisplayStringLine(LCD_LINE_5, (uint8_t*) "Try again after res-");
  LCD_DisplayStringLine(LCD_LINE_6, (uint8_t*) "  etting the Board  ");
#endif /* ENABLE_LCD_MSG_DISPLAY */
  
  /* Block communication and all processes */
  while (1)
  {   
  }  
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;  
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

