/**
  @page I2S_FullDuplexDataExchangeDMA I2S Full-Duplex with DMA data transfer example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    I2S/I2S_FullDuplexDataExchangeDMA/readme.txt 
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    15-December-2021
  * @brief   Description of the I2S Full-Duplex data exchange with DMA example.
  ******************************************************************************
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
   @endverbatim

@par Example Description 

This example provides a description of how to set a communication between two
SPIs in I2S Full-Duplex mode using DMA and performing a transfer from Master to 
Slave.

- Hardware Description

To use this example, you need to load it on two STM32 boards (let's call them 
Board A and Board B) then connect these two boards through SPI lines and GND.
In the firmware example uncomment the dedicated line in the main.h file  to use
the SPI peripheral as STM32 Master device or as STM32 Slave .

@verbatim
*------------------------------------------------------------------------------*
|                BOARD A                                BOARD B                |
|                Master                                 Slave                  |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         | 
|        |     __________     |                 |     __________     |         |
|        |    | I2Sext   |    |                 |    | I2Sext   |    |         |                                
|        |    | Slave Rx |____|__SDin(I2Sext)___|____| Slave Tx |    |         |
|        |    |__________|    |                 |    |__________|    |         |
|        |    |SPI Master|____|______SDout______|____| SPI Slave|    |         |
|        |    |Tx Device |____|_______WS________|____| Rx Device|    |         |
|        |    |__________|____|_______CK________|____|__________|    |         |
|        |                    |                 |                    |         |
|        |                    |                 |  O LD1             |         |
|        |              Key   |                 |  O LD2             |         |
|        |               _    |                 |  O LD3             |         |
|        |              |_|   |                 |  O LD4             |         |
|        |                    |                 |                    |         |
|        |             GND O--|-----------------|--O GND             |         |
|        |____________________|                 |____________________|         |
|                                                                              |
|                                                                              |
*------------------------------------------------------------------------------*
@endverbatim

- Software Description

In master board, I2S3 peripheral is configured as Master Transmitter, whereas I2S3ext
is configured as a slave Receiver.
In Slave board, I2S3 peripheral is configured as Slave Receiver, whereas I2S3ext
is configured as a slave Transmitter.
The data transfer is managed with DMA. 
Both devices are in Philips standard configuration with 16 bit and 48KHz audio frequency. 

This example describes how to Configure I2S3 in Full duplex mode and send/Receive 
in the meantime I2S_Buffer_Tx and I2S_Buffer_Rx or I2Sext_Buffer_Rx buffer from 
(I2S3:BoradA) to (I2S3:BoardB).

The I2S_Buffer_Tx is transmitted using DMA2 channel2 and the received values are 
loaded in the I2S_Buffer_Rx buffer or I2Sext_Buffer_Rx buffer using DMA2 channel1. 

When the user press on the Key Button on the Master Board(BoardA), the master 
start sending data stored in the I2S_Buffer_Tx buffer while the Slave Board (BoardB)
receives data in I2S_Buffer_Rx buffer.
At the same time, the Slave Board(BoardB) send data stored in the I2S_Buffer_Tx 
buffer while the BoardB receives data in I2Sext_Buffer_Rx buffer.

Once the transfer is completed, a comparison is done and TransferStatus gives the 
data transfer status:
  - TransferStatus PASSED if transmitted and received data are the same: Green LED is On.
  - TransferStatus FAILED if transmitted and received data are the different: Red LED is On.

@par Directory contents 

  - I2S/I2S_FullDuplexDataExchangeDMA/stm32f30x_conf.h    Library Configuration file
  - I2S/I2S_FullDuplexDataExchangeDMA/stm32f30x_it.c      Interrupt handlers
  - I2S/I2S_FullDuplexDataExchangeDMA/stm32f30x_it.h      Interrupt handlers header file
  - I2S/I2S_FullDuplexDataExchangeDMA/main.c              Main program
  - I2S/I2S_FullDuplexDataExchangeDMA/main.h              Header for main.c module
  - I2S/I2S_FullDuplexDataExchangeDMA/system_stm32f30x.c  STM32F30x system source file
  
@note The "system_stm32f30x.c" is generated by an automatic clock configuration 
      system and can be easily customized to your own configuration. 
      To select different clock setup, use the "STM32F30x_Clock_Configuration_V1.0.0.xls" 
      provided with the AN4152 package available on <a href="http://www.st.com/internet/mcu/family/141.jsp">  ST Microcontrollers </a>
     
@par Hardware and Software environment

  - This example runs on STM32F303xC and STM32F303xE Devices.
  
  - This example has been tested with STMicroelectronics STM32303C-EVAL (STM32F30x)
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM32303C-EVAL Set-up
    - Connect I2S3_SCK pin (PC.10) toI2S3_SCK pin (PC.10)
    - Connect I2S3_Din pin (PC.11) to I2S3_Din pin (PC.11)
    - Connect I2S3_Dout pin (PC.12) to I2S3_Dout pin (PC.12)
    - Connect I2S3_WS pin (PA.04) to I2S3_WS pin (PA.04)
    - The connection between the pins should use a short wires and a common Ground.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Projects\STM32F30x_StdPeriph_Templates
 - Open your preferred toolchain 
 - Add the following file the project source list
    - Utilities\STM32_EVAL\STM32303C_EVAL\stm32303c_eval.c
 - Rebuild all files and load your image into target memory
 - Run the example
    

 */
