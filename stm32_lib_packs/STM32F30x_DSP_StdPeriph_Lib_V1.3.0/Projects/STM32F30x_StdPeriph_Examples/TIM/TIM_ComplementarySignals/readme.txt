/**
  @page TIM_ComplementarySignals TIM complementary signals Example Description
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    TIM/TIM_ComplementarySignals/readme.txt 
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    15-December-2021
  * @brief   TIM complementary signals Example Description.
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

This example shows how to configure the TIM1 peripheral to generate three 
complementary TIM1 signals, to insert a defined dead time value, to use the break 
feature and to lock the desired parameters.

TIM1CLK is fixed to SystemCoreClock, the TIM1 Prescaler is equal to 0 so the 
TIM1 counter clock used is SystemCoreClock (72MHz).

The objective is to generate PWM signal at 17.57 KHz:
  - TIM1_Period = (SystemCoreClock / 17570) - 1

The Three Duty cycles are computed as the following description: 
The channel 1 duty cycle is set to 50% so channel 1N is set to 50%.
The channel 2 duty cycle is set to 25% so channel 2N is set to 75%.
The channel 3 duty cycle is set to 12.5% so channel 3N is set to 87.5%.
The Timer pulse is calculated as follows:
  - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100

A dead time equal to 11/SystemCoreClock is inserted between the different 
complementary signals, and the Lock level 1 is selected.
The break Polarity is used at High level.

The TIM1 waveform can be displayed using an oscilloscope. 

@par Directory contents 

  - TIM/TIM_ComplementarySignals/stm32f30x_conf.h    Library Configuration file
  - TIM/TIM_ComplementarySignals/stm32f30x_it.c      Interrupt handlers
  - TIM/TIM_ComplementarySignals/stm32f30x_it.h      Interrupt handlers header file
  - TIM/TIM_ComplementarySignals/main.c              Main program
  - TIM/TIM_ComplementarySignals/system_stm32f30x.c  STM32F30x system source file
  
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
    - Connect the TIM1 pins to an oscilloscope to monitor the different waveforms:
      - TIM1_CH1  pin (PA.08)  
      - TIM1_CH1N pin (PA.07)  
      - TIM1_CH2  pin (PA.09)  
      - TIM1_CH2N pin (PB.00)  
      - TIM1_CH3  pin (PA.10)  
      - TIM1_CH3N pin (PB.01)

    - Connect the TIM1 break pin TIM1_BKIN pin (PA.06) to the GND. To generate a 
      break event, switch this pin level from 0V to 3.3V.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Projects\STM32F30x_StdPeriph_Templates
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
    

 */
