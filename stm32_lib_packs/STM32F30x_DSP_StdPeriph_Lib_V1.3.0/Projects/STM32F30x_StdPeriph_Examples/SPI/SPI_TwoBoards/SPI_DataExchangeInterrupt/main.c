/**
  ******************************************************************************
  * @file    SPI/SPI_TwoBoards/SPI_DataExchangeInterrupt/main.c
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
  
/** @addtogroup SPI_DataExchangeInterrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SPI_InitTypeDef  SPI_InitStructure;

uint8_t TxBuffer[] = "SPI Interrupt Example: Communication between two SPI using Interrupts";
uint8_t RxBuffer [RXBUFFERSIZE];

__IO uint32_t Rx_Idx = 0x00;
__IO uint32_t Tx_Idx = 0x00;

__IO JOYState_TypeDef PressedButton  = JOY_NONE;
__IO uint8_t CmdTransmitted = 0x00;
__IO uint32_t CmdReceived = 0x00;
__IO uint32_t CmdStatus = 0x00;
__IO uint32_t TimeOut = 0x0;

/* Private function prototypes -----------------------------------------------*/
static void SPI_Config(void);
static void SysTickConfig(void);
static void TimeOut_UserCallback(void);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength, uint8_t DataMask);
static void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLength);
#ifdef SPI_MASTER
static JOYState_TypeDef Read_Joystick(void);
#endif

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
  
  /* SPI configuration ------------------------------------------------------*/
  SPI_Config();
  
  /* SysTick configuration ---------------------------------------------------*/
  SysTickConfig();
  
  /* Initialize LEDs mounted on STM32303C-EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  /* Master board configuration ------------------------------------------------*/
#ifdef SPI_MASTER
  /* Initialize push-buttons mounted on STM32303C-EVAL board */
  STM_EVAL_PBInit(BUTTON_RIGHT, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_LEFT, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_GPIO);
  
  /* Initializes the SPI communication */
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPIx, &SPI_InitStructure);
  
  /* Initialize the FIFO threshold */
  SPI_RxFIFOThresholdConfig(SPIx, SPI_RxFIFOThreshold_QF);
  
  /* Enable the Rx buffer not empty interrupt */
  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_RXNE, ENABLE);
  /* Enable the SPI Error interrupt */
  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_ERR, ENABLE);
  /* Data transfer is performed in the SPI interrupt routine */
  
  /* Enable the SPI peripheral */
  SPI_Cmd(SPIx, ENABLE);
  
  while (1)
  {   
    CmdTransmitted = 0x00;
    CmdReceived = 0x00;
    CmdStatus = 0x00;
    Tx_Idx = 0x00;
    Rx_Idx = 0x00;
    
    /* Clear the RxBuffer */
    Fill_Buffer(RxBuffer, TXBUFFERSIZE);
    PressedButton = Read_Joystick();
    
    while (PressedButton == JOY_NONE)
    {
      PressedButton = Read_Joystick();
    }    
    
    switch (PressedButton)
    {
      /* JOY_RIGHT button pressed */
    case JOY_RIGHT:
      CmdTransmitted = CMD_RIGHT;
      break;
      /* JOY_LEFT button pressed */ 
    case JOY_LEFT:
      CmdTransmitted = CMD_LEFT;
      break;
      /* JOY_UP button pressed */
    case JOY_UP:
      CmdTransmitted = CMD_UP;
      break;
      /* JOY_DOWN button pressed */
    case JOY_DOWN:
      CmdTransmitted = CMD_DOWN;
      break;
    default:
      break;
    }
    
    if (CmdTransmitted != 0x00)
    {    
      /* Enable the Tx buffer empty interrupt */
      SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, ENABLE);
      
      /* Wait until end of data transfer or time out*/
      TimeOut = USER_TIMEOUT;
      while ((Rx_Idx < DATA_SIZE)&&(TimeOut != 0x00))
      {}
      if(TimeOut == 0)
      {
        TimeOut_UserCallback();
      } 
    }
    
    /* Waiting until TX FIFO is empty */
    while (SPI_GetTransmissionFIFOStatus(SPIx) != SPI_TransmissionFIFOStatus_Empty)
    {}
    
    /* Wait busy flag */
    while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)
    {}
    
    /* Waiting until RX FIFO is empty */
    while (SPI_GetReceptionFIFOStatus(SPIx) != SPI_ReceptionFIFOStatus_Empty)
    {}
    
    switch (CmdTransmitted)
    {
      /* Right button pressed */
    case CMD_RIGHT:
      if ((Buffercmp(TxBuffer, RxBuffer, DATA_SIZE, SPI_DATAMASK) == PASSED) && (CmdReceived == CMD_ACK))
      {
        /* Turn ON LED2 and LED3 */
        STM_EVAL_LEDOn(LED2);
        STM_EVAL_LEDOn(LED3);
        /* Turn all other LEDs off */
        STM_EVAL_LEDOff(LED4);
      }
      break;
      /* Left button pressed*/
    case CMD_LEFT:
      if ((Buffercmp(TxBuffer, RxBuffer, DATA_SIZE, SPI_DATAMASK) == PASSED) && (CmdReceived == CMD_ACK))
      {
        /* Turn ON LED4 */
        STM_EVAL_LEDOn(LED4);
        /* Turn all other LEDs off */
        STM_EVAL_LEDOff(LED2);
        STM_EVAL_LEDOff(LED3);
      }
      break;
      /* Up button pressed */
    case CMD_UP:
      if ((Buffercmp(TxBuffer, RxBuffer, DATA_SIZE, SPI_DATAMASK) == PASSED) && (CmdReceived == CMD_ACK))
      {
        /* Turn ON LED2 */
        STM_EVAL_LEDOn(LED2);
        /* Turn all other LEDs off */
        STM_EVAL_LEDOff(LED3);
        STM_EVAL_LEDOff(LED4);
      }
      break;
      /* Down button pressed */
    case CMD_DOWN:
      if ((Buffercmp(TxBuffer, RxBuffer, DATA_SIZE, SPI_DATAMASK) == PASSED) && (CmdReceived == CMD_ACK))
      {
        /* Turn ON LED3 */
        STM_EVAL_LEDOn(LED3);
        /* Turn all other LEDs off */
        STM_EVAL_LEDOff(LED2);
        STM_EVAL_LEDOff(LED4);
      }
      break;
    default:
      break;
    }        
  }
#endif /* SPI_MASTER */
  
  /* Slave board configuration ----------------------------------------------*/
#ifdef SPI_SLAVE
  /* Initializes the SPI communication */
  SPI_I2S_DeInit(SPIx);
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_Init(SPIx, &SPI_InitStructure);
  
  /* Initialize the FIFO threshold */
  SPI_RxFIFOThresholdConfig(SPIx, SPI_RxFIFOThreshold_QF);
  
  /* Enable the Rx buffer not empty interrupt */
  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_RXNE, ENABLE);
  
  /* Enable the SPI Error interrupt */
  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_ERR, ENABLE);
  
  /* Enable the SPI peripheral */
  SPI_Cmd(SPIx, ENABLE);
  
  /* Infinite Loop */
  while (1)
  {     
    CmdStatus = 0x00;
    CmdReceived = 0x00;
    Rx_Idx = 0x00;
    Tx_Idx = 0x00;
    
    /* Write the first data in SPI shift register before enabling the interrupt
    this data will be transmitted when the Slave receive the generated clock
    by the Master */    
    /* Enable the Tx buffer empty interrupt */
    SPI_SendData8(SPIx, CMD_ACK);
    SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, ENABLE);
    
    /* Waiting Transaction code Byte */
    while (CmdStatus == 0x00)
    {}
    
    TimeOut = USER_TIMEOUT;
    while ((Rx_Idx < DATA_SIZE)&&(TimeOut != 0x00))
    {}
    if(TimeOut == 0)
    {
      TimeOut_UserCallback();
    }    
    
    switch (CmdReceived)
    {
      /* CMD_RIGHT command received or time out*/
    case CMD_RIGHT:
      if (Buffercmp(TxBuffer, RxBuffer, DATA_SIZE, SPI_DATAMASK) != FAILED) 
      {
        /* Turn ON LED2 and LED3 */
        STM_EVAL_LEDOn(LED2);
        STM_EVAL_LEDOn(LED3);
        /* Turn OFF LED4 */
        STM_EVAL_LEDOff(LED4);
      }
      break;
      /* CMD_LEFT command received or time out */
    case CMD_LEFT:
      if (Buffercmp(TxBuffer, RxBuffer, DATA_SIZE, SPI_DATAMASK) != FAILED)
      {
        /* Turn ON LED4 */
        STM_EVAL_LEDOn(LED4);
        /* Turn OFF LED2 and LED3 */
        STM_EVAL_LEDOff(LED2);
        STM_EVAL_LEDOff(LED3);
      }
      break;
      /* CMD_UP command received or time out*/
    case CMD_UP:
      if (Buffercmp(TxBuffer, RxBuffer, DATA_SIZE, SPI_DATAMASK) != FAILED)
      {
        /* Turn ON LED2 */
        STM_EVAL_LEDOn(LED2);
        /* Turn OFF LED3 and LED4 */
        STM_EVAL_LEDOff(LED3);
        STM_EVAL_LEDOff(LED4);
      }
      break;
      /* CMD_DOWN command received or time out */
    case CMD_DOWN:
      if (Buffercmp(TxBuffer, RxBuffer, DATA_SIZE, SPI_DATAMASK) != FAILED)
      {
        /* Turn ON LED3 */
        STM_EVAL_LEDOn(LED3);
        /* Turn OFF LED2 and LED4 */
        STM_EVAL_LEDOff(LED2);
        STM_EVAL_LEDOff(LED4);
      }
      break;
    default:
      break;   
    }
    
    /* Clear the RxBuffer */
    Fill_Buffer(RxBuffer, TXBUFFERSIZE);
    
    /* Waiting until TX FIFO is empty */
    while (SPI_GetTransmissionFIFOStatus(SPIx) != SPI_TransmissionFIFOStatus_Empty)
    {}    
    
    /* Wait busy flag */
    while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)
    {}
    
    /* Disable the Tx buffer empty interrupt */
    SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, DISABLE);
    
    /* Waiting until RX FIFO is empty */
    while (SPI_GetReceptionFIFOStatus(SPIx) != SPI_ReceptionFIFOStatus_Empty)
    {} 
    
  }
#endif /* SPI_SLAVE */
}

/**
  * @brief  Returns NbrOfDataToTransfer value.
  * @param  None
  * @retval Tx Buffer Size (NbrOfDataToTransfer1).
  */
uint8_t GetVar_NbrOfData(void)
{
  return DATA_SIZE;
}

/**
* @brief  Basic management of the timeout situation.
* @param  None.
* @retval None.
*/
static void TimeOut_UserCallback(void)
{
  /* User can add his own implementation to manage TimeOut Communication failure */
  /* Block communication and all processes */
  while (1)
  {   
  }
}

/**
  * @brief  Configures the SPI Peripheral.
  * @param  None
  * @retval None
  */
static void SPI_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable the SPI periph */
  RCC_APB1PeriphClockCmd(SPIx_CLK, ENABLE);
  
  /* Enable SCK, MOSI, MISO and NSS GPIO clocks */
  RCC_AHBPeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK, ENABLE);
  
  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
  GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);
  GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIx_MISO_PIN;
  GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);
    
  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SPIx);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DATASIZE;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  
  /* Configure the Priority Group to 1 bit */                
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Configure the SPI interrupt priority */
  NVIC_InitStructure.NVIC_IRQChannel = SPIx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configure a SysTick Base time to 10 ms.
  * @param  None
  * @retval None
  */
static void SysTickConfig(void)
{
  /* Setup SysTick Timer for 10ms interrupts  */
  if (SysTick_Config(SystemCoreClock / 100))
  {
    /* Capture error */
    while (1);
  }

  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength, uint8_t DataMask)
{
  while (BufferLength--)
  {
    if (((*pBuffer1) & DataMask) != *pBuffer2)
    {
      return FAILED;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

/**
  * @brief  Fills buffer.
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferLength: size of the buffer to fill
  * @retval None
  */
static void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLength)
{
  uint16_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLength; index++ )
  {
    pBuffer[index] = 0x00;
  }
}

#ifdef SPI_MASTER
/**
* @brief  Reads key from evaluationboard.
* @param  None
* @retval Return JOY_RIGHT, JOY_LEFT, JOY_UP, JOY_DOWN or JOY_NONE
*/
static JOYState_TypeDef Read_Joystick(void)
{
  /* "JOY_RIGHT" key is pressed */
  if (STM_EVAL_PBGetState(BUTTON_RIGHT)== Bit_SET)
  {
    while (STM_EVAL_PBGetState(BUTTON_RIGHT) == RESET)
    {}
    return JOY_RIGHT;
  }
  /* "JOY_LEFT" key is pressed */
  if (STM_EVAL_PBGetState(BUTTON_LEFT)==Bit_SET)
  {
    while (STM_EVAL_PBGetState(BUTTON_LEFT) == RESET)
    {}
    return JOY_LEFT;
  }
  /* "JOY_UP" key is pressed */
  if (STM_EVAL_PBGetState(BUTTON_UP)==Bit_SET)
  {
    while (STM_EVAL_PBGetState(BUTTON_UP) == RESET)
    {}
    return JOY_UP;
  }
  /* "JOY_DOWN" key is pressed */
  if (STM_EVAL_PBGetState(BUTTON_DOWN)==Bit_SET)
  {
    while (STM_EVAL_PBGetState(BUTTON_DOWN) == RESET)
    {}
    return JOY_DOWN;
  }
  /* No key is pressed */
  else
  {
    return JOY_NONE;
  }
}

#endif

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
  


