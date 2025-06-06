/**
  @page I2C_TSENSOR Communication between I2C and STTS751 Temperature sensor
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    I2C/I2C_TSENSOR/readme.txt 
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    15-December-2021
  * @brief   Description of the I2C TSENSOR example.
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

This example provides a description of how to use the I2C to communicate with 
an STTS751 (or a compatible device) I2C temperature sensor is mounted on the 
STM32303C-EVAL board and used to get instantaneous external temperature (-55�C to +125�C).
 
Thanks to STM32 I2C SMBus feature, we can easily monitor the temperature 
variations. This is managed by the SMBus Alert which is generating a dedicated 
interrupt informing the system that the temperature is out of the selected 
ranges.
The user can configure the TOS and THYS thanks to a dedicated define values in 
the code.

By default the STM32303C-EVAL demo are setting them to (see main.c file):
  
#define TEMPERATURE_THYS        0x14 /* 20�C */
#define TEMPERATURE_TOS         0x28 /* 40�C */

@par Directory contents 

  - I2C/I2C_TSENSOR/stm32f30x_conf.h    Library Configuration file
  - I2C/I2C_TSENSOR/stm32f30x_it.c      Interrupt handlers
  - I2C/I2C_TSENSOR/stm32f30x_it.h      Interrupt handlers header file
  - I2C/I2C_TSENSOR/main.c              Main program
  - I2C/I2C_TSENSOR/main.h              Main program header file   
  - I2C/I2C_TSENSOR/system_stm32f30x.c  STM32F30x system source file
  
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
    - Make sure that JP5 and JP6 jumpers are fitted in position 1-2.
    - Make sure that JP1 jumper is fitted.
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Projects\STM32F30x_StdPeriph_Templates
 - Open your preferred toolchain 
 - Add the following file the project source list
    - Utilities\STM32_EVAL\STM32303C_EVAL\stm32303c_eval.c
    - Utilities\STM32_EVAL\STM32303C_EVAL\stm32303c_eval_lcd.c
    - Utilities\STM32_EVAL\STM32303C_EVAL\stm32303c_eval_i2c_tsensor.c
 - Before building the project please make sure that "USE_DEFAULT_TIMEOUT_CALLBACK" 
   define is removed from Preprocessor defines under C/C++ compiler settings. 
 - Rebuild all files and load your image into target memory
 - Run the example
   

 */
