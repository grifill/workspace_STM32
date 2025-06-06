/**
  @page TIM_DMABurst TIM DMA burst Example Description
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    TIM/TIM_DMABurst/readme.txt 
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    15-December-2021
  * @brief   TIM DMA burst Example Description.
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

This example shows how to update the TIM1 channel1 period and the duty cycle 
using the TIM DMA burst feature.

Every update DMA request, the DMA will do 3 transfers of half words into Timer 
registers beginning from ARR register.
On the DMA update request, 0x0FFF will be transferred into ARR, 0x0000 
will be transferred into RCR, 0x0555 will be transferred into CCR1. 

The TIM1CLK frequency is set to SystemCoreClock (Hz), to get TIM1 counter
clock at 36 MHz the Prescaler is computed as following:
   - Prescaler = (TIM1CLK / TIM1 counter clock) - 1

SystemCoreClock is set to 72 MHz.

The TIM1 period is 8.79 KHz: TIM1 Frequency = TIM1 counter clock/(ARR + 1)
                                           = 36 MHz / 4096 = 8.79 KHz

The TIM1 CCR1 register value is equal to 0x555, so the TIM1 Channel 1 generates a 
PWM signal with a frequency equal to 8.79 KHz and a duty cycle equal to 33.33%:
TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR + 1)* 100 = 33.33%

The PWM waveform can be displayed using an oscilloscope.
 
@par Directory contents 

  - TIM/TIM_DMABurst/stm32f30x_conf.h    Library Configuration file
  - TIM/TIM_DMABurst/stm32f30x_it.c      Interrupt handlers
  - TIM/TIM_DMABurst/stm32f30x_it.h      Interrupt handlers header file
  - TIM/TIM_DMABurst/main.c              Main program
  - TIM/TIM_DMABurst/system_stm32f30x.c  STM32F30x system source file
  
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
    - Connect TIM1 CH1 (PA.08) to an oscilloscope to monitor the waveforms.  
 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Projects\STM32F30x_StdPeriph_Templates
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
    

 */
