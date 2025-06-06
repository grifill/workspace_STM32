/**
  @page FLASH_WriteProtection FLASH_Write_Protection
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    FLASH/FLASH_WriteProtection/readme.txt 
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    15-December-2021
  * @brief   FLASH Write_Protection Description.
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

This example provides a description of how to enable and disable the write protection
for the STM32F30x FLASH:

- Enable Write protection: 
   To enable the Write Protection, uncomment the line "#define WRITE_PROTECTION_ENABLE"
   in main.c file.  
   To protect a set of pages, the user has to call the function FLASH_EnableWriteProtection.
   The parameter of this function will define the number of pages to be protected.
   To load the new option byte values, a system Reset is necessary, for this, the
   function FLASH_OB_Launch() is used.
 
- Disable Write protection:
   To disable the Write Protection, uncomment the line "#define WRITE_PROTECTION_DISABLE"
   in main.c file.
   To disable the write protection of the STM32F30x Flash, an erase of the Option 
   Bytes is necessary. This operation is done by the function FLASH_EraseOptionBytes.
   To load the new option byte values, a system Reset is necessary, for this, the
   function FLASH_OB_Launch() is used.

If the desired pages are not write protected, an erase and a write operation are
performed.

@par Directory contents 

  - FLASH/FLASH_WriteProtection/stm32f30x_conf.h     Library Configuration file
  - FLASH/FLASH_WriteProtection/stm32f30x_it.h       Interrupt handlers header file
  - FLASH/FLASH_WriteProtection/stm32f30x_it.c       Interrupt handlers
  - FLASH/FLASH_WriteProtection/main.c               Main program
  - FLASH/FLASH_WriteProtection/system_stm32f30x     STM32F30x system source file
  
@note The "system_stm32f30x.c" is generated by an automatic clock configuration 
      system and can be easily customized to your own configuration. 
      To select different clock setup, use the "STM32F30x_Clock_Configuration_V1.0.0.xls" 
      provided with the AN4152 package available on <a href="http://www.st.com/internet/mcu/family/141.jsp">  ST Microcontrollers </a>
 
@par Hardware and Software environment 

  - This example runs on STM32F303xC and STM32F303xE Devices.
  
  - This example has been tested with STMicroelectronics STM32303C-EVAL (STM32F30x)
    evaluation board and can be easily tailored to any other supported device 
    and development board.

  
@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Projects\STM32F30x_StdPeriph_Templates
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 

 */
