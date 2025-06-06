/**
  @page TIM_6Steps TIM 6 Steps example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    TIM/TIM_6Steps/readme.txt 
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    15-December-2021
  * @brief   TIM 6Steps Description.
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

This example shows how to configure the TIM1 peripheral to generate 6 Steps.
The STM32F30x TIM1 peripheral offers the possibility to program in advance the 
configuration for the next TIM1 outputs behaviour (step) and change the configuration
of all the channels at the same time. This operation is possible when the COM 
(commutation) event is used.
The COM event can be generated by software by setting the COM bit in the TIM1_EGR
register or by hardware (on TRC rising edge).
In this example, a software COM event is generated each 100 ms: using the SysTick 
interrupt.
The TIM1 is configured in Timing Mode, each time a COM event occurs, a new TIM1
configuration will be set in advance.

The break Polarity is used at High level.

The following Table describes the TIM1 Channels states:
 @verbatim
              -----------------------------------------------
             | Step1 | Step2 | Step3 | Step4 | Step5 | Step6 |
   ----------------------------------------------------------
  |Channel1  |   1   |   0   |   0   |   0   |   0   |   1   |
   ----------------------------------------------------------
  |Channel1N |   0   |   0   |   1   |   1   |   0   |   0   |
   ----------------------------------------------------------
  |Channel2  |   0   |   0   |   0   |   1   |   1   |   0   |
   ----------------------------------------------------------
  |Channel2N |   1   |   1   |   0   |   0   |   0   |   0   |
   ----------------------------------------------------------
  |Channel3  |   0   |   1   |   1   |   0   |   0   |   0   |
   ----------------------------------------------------------
  |Channel3N |   0   |   0   |   0   |   0   |   1   |   1   |
   ----------------------------------------------------------
 

@par Directory contents 

  - TIM/TIM_6Steps/stm32f30x_conf.h    Library Configuration file
  - TIM/TIM_6Steps/stm32f30x_it.c      Interrupt handlers
  - TIM/TIM_6Steps/stm32f30x_it.h      Interrupt handlers header file
  - TIM/TIM_6Steps/main.c              Main program
  - TIM/TIM_6Steps/main.h              header file Main program
  - TIM/TIM_6Steps/system_stm32f30x.c  STM32F30x system source file
  
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
      - TIM1_CH3  pin (PE.13)
      - TIM1_CH1N pin (PC.13)  
      - TIM1_CH2  pin (PA.09)  
      - TIM1_CH3N pin (PB.01)
      - TIM1_CH1  pin (PA.08)  
      - TIM1_CH2N pin (PB.00)    
    - Connect the TIM1 break pin TIM1_BKIN pin (PA.06) to the 3.3V. To generate a 
      break event, switch this pin level from 3.3V to 0V. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Projects\STM32F30x_StdPeriph_Templates
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
