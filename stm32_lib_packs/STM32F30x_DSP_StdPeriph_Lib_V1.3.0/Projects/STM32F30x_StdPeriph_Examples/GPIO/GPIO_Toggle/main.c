/**
  ******************************************************************************
  * @file    GPIO/GPIO_Toggle/main.c 
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
#include "stm32f30x.h"

/** @addtogroup STM32F30x_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_Toggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BSRR_VAL 0xC000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef        GPIO_InitStructure;
/* Private function prototypes -----------------------------------------------*/
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

 /* GPIOE Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  
  /* Configure PE14 and PE15 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  /* To achieve GPIO toggling maximum frequency, the following  sequence is mandatory. 
  You can monitor PE14 and PE15 on the scope to measure the output signal. 
  If you need to fine tune this frequency, you can add more GPIO set/reset 
  cycles to minimize more the infinite loop timing.
  This code needs to be compiled with high speed optimization option.  */
  while (1)
  {
    /* Set PE14 and PE15 */
    GPIOE->BSRR = BSRR_VAL;
    /* Reset PE14 and PE15 */
    GPIOE->BRR = BSRR_VAL;
    
    /* Set PE14 and PE15 */
    GPIOE->BSRR = BSRR_VAL;
    /* Reset PE14 and PE15 */
    GPIOE->BRR = BSRR_VAL;
    
    /* Set PE14 and PE15 */
    GPIOE->BSRR = BSRR_VAL;
    /* Reset PE14 and PE15 */
    GPIOE->BRR = BSRR_VAL;
    
    /* Set PE14 and PE15 */
    GPIOE->BSRR = BSRR_VAL;
    /* Reset PE14 and PE15 */
    GPIOE->BRR = BSRR_VAL;
    
    /* Set PE14 and PE15 */
    GPIOE->BSRR = BSRR_VAL;
    /* Reset PE14 and PE15 */
    GPIOE->BRR = BSRR_VAL;
    
    /* Set PE14 and PE15 */
    GPIOE->BSRR = BSRR_VAL;
    /* Reset PE14 and PE15 */
    GPIOE->BRR = BSRR_VAL;
    
    /* Set PE14 and PE15*/
    GPIOE->BSRR = BSRR_VAL;
    /* Reset PE14 and PE15 */
    GPIOE->BRR = BSRR_VAL;
    
    /* Set PE14 and PE15 */
    GPIOE->BSRR = BSRR_VAL;
    /* Reset PE14 and PE15 */
    GPIOE->BRR = BSRR_VAL;
    
    /* Set PE14 and PE15 */
    GPIOE->BSRR = BSRR_VAL;
    /* Reset PC0 and PC1 */
    GPIOE->BRR = BSRR_VAL;
    
    /* Set PE14 and PE15 */
    GPIOE->BSRR = BSRR_VAL;
    /* Reset PE14 and PE15 */
    GPIOE->BRR = BSRR_VAL;
  }
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

