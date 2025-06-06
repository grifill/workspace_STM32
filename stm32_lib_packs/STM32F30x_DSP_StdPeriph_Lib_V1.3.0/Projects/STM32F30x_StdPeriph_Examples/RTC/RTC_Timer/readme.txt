/**
  @page RTC_Timer RTC Timer example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    RTC/RTC_Timer/readme.txt 
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    15-December-2021
  * @brief   Description of the RTC Timer example.
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

  This example provides a short description of how to use the RTC peripheral�s with 
  Alarm sub seconds feature to simulate a timer with refresh time equal to 250 ms
  (1 second/ 8) * 2).
  The RTC is configured to generate sub seconds interrupt each 125ms (will have
  8 interrupt per 1 second).
  
  For this example an interactive human interface is developed using
  LCD and Push Buttons to allow user to use Timer with a real "sand timer"
  display.

  After startup, the sand timer is equal to 1 Minute (60 Seconds) and by pressing on
  the RIGHT button the bottom bulb start to be filled for each 2 successive
  interrupts (after each 250ms).
  After 480 interrupts (60 * 8) the bottom bulb will be full.

  User can manipulate the chronometer features using the Joystick SEL, RIGHT, UP and DOWN buttons:
    - press Joystick SEL button to Start the timer.
    - press again Joystick SEL button to stop the timer.
    - press Joystick RIGHT button to Restart the timer.
    - press Joystick UP/DOWN button to adjust the timer. 

@par Directory contents 

  - RTC/RTC_Timer/stm32f30x_conf.h    Library Configuration file
  - RTC/RTC_Timer/stm32f30x_it.c      Interrupt handlers
  - RTC/RTC_Timer/stm32f30x_it.h      Interrupt handlers header file
  - RTC/RTC_Timer/main.c              Main program
  - RTC/RTC_Timer/main.h              Main program header file
  - RTC/RTC_Timer/system_stm32f30x.c  STM32F30x system source file
  
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
    - None.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Projects\STM32F30x_StdPeriph_Templates
 - Open your preferred toolchain 
 - Add the following file the project source list
   - Utilities\STM32_EVAL\STM32303C_EVAL\stm32303c_eval.c
   - Utilities\STM32_EVAL\STM32303C_EVAL\stm32303c_eval_lcd.c
 - Rebuild all files and load your image into target memory
 - Run the example
    

 */
