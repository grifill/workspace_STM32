/**
  ******************************************************************************
  * @file    FLASH/FLASH_WriteProtection/main.c 
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
  
/** @addtogroup FLASH_WriteProtection
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/
#ifdef STM32F303xE
#define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#define FLASH_PAGES_TO_BE_PROTECTED (OB_WRP_Pages24to25 | OB_WRP_Pages26to27 | OB_WRP_Pages60to61 | OB_WRP_Pages62to263)  

#define BANK1_WRITE_START_ADDR  ((uint32_t)0x08006000)
#define BANK1_WRITE_END_ADDR    ((uint32_t)0x08080000)
#else
#define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#define FLASH_PAGES_TO_BE_PROTECTED (OB_WRP_Pages24to25 | OB_WRP_Pages26to27)  

#define BANK1_WRITE_START_ADDR  ((uint32_t)0x08006000)
#define BANK1_WRITE_END_ADDR    ((uint32_t)0x08008000)
#endif /* STM32F303xE */
 
/* Uncomment this line to program the Flash pages */
#define FLASH_PAGE_PROGRAM

/* Uncomment this line to Enable Write Protection */
/* #define WRITE_PROTECTION_ENABLE */

/* Uncomment this line to Disable Write Protection */
/* #define WRITE_PROTECTION_DISABLE */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t EraseCounter = 0x0, Address = 0x0;
uint16_t Data = 0x1753;
uint32_t WRPR_Value = 0xFFFFFFFF, ProtectedPages = 0x0;
uint32_t NbrOfPage;
__IO FLASH_Status FLASHStatus = FLASH_COMPLETE;
__IO TestStatus MemoryProgramStatus = PASSED;
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
  
  /* Unlock the Flash Program Erase controller */  
  FLASH_Unlock();
  
  FLASH_OB_Unlock();
  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
    
  /* Get pages write protection status */
  WRPR_Value = FLASH_OB_GetWRP();

#ifdef WRITE_PROTECTION_DISABLE
  
  /* Get pages already write protected */
  ProtectedPages = ~(WRPR_Value | FLASH_PAGES_TO_BE_PROTECTED);
  
  /* Check if desired pages are already write protected */
  if((WRPR_Value | (~FLASH_PAGES_TO_BE_PROTECTED)) != 0xFFFFFFFF )
  {
    /* Erase all the option Bytes */
    FLASHStatus = FLASH_OB_Erase();
    
  /* Check if there is write protected pages */
  if(ProtectedPages != 0x0)
  {
      /* Restore write protected pages */
      FLASHStatus = FLASH_OB_EnableWRP(ProtectedPages);
    }
    /* Generate System Reset to load the new option byte values */
    FLASH_OB_Launch();
  }

#elif defined WRITE_PROTECTION_ENABLE
  /* Get current write protected pages and the new pages to be protected */
  ProtectedPages =  (~WRPR_Value) | FLASH_PAGES_TO_BE_PROTECTED; 
  
  /* Check if desired pages are not yet write protected */
  if(((~WRPR_Value) & FLASH_PAGES_TO_BE_PROTECTED )!= FLASH_PAGES_TO_BE_PROTECTED)
  {
  
    /* Erase all the option Bytes because if a program operation is 
      performed on a protected page, the Flash memory returns a 
      protection error */
    FLASHStatus = FLASH_OB_Erase();

    /* Enable the pages write protection */
    FLASHStatus = FLASH_OB_EnableWRP(ProtectedPages);
  
    /* Generate System Reset to load the new option byte values */
    FLASH_OB_Launch();
  }
#endif /* WRITE_PROTECTION_DISABLE */

#ifdef FLASH_PAGE_PROGRAM  
  /* Get the number of pages to be erased */
  NbrOfPage = (BANK1_WRITE_END_ADDR - BANK1_WRITE_START_ADDR) / FLASH_PAGE_SIZE;

  /* The selected pages are not write protected */
  if ( (WRPR_Value & FLASH_PAGES_TO_BE_PROTECTED) != 0x00)
  {
    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP|FLASH_FLAG_PGERR |FLASH_FLAG_WRPERR);	

    /* erase the FLASH pages */
    for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
    {
      FLASHStatus = FLASH_ErasePage(BANK1_WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
    }
  
    /* FLASH Half Word program of data 0x1753 at addresses defined by  BANK1_WRITE_START_ADDR and BANK1_WRITE_END_ADDR */
    Address = BANK1_WRITE_START_ADDR;

    while((Address < BANK1_WRITE_END_ADDR) && (FLASHStatus == FLASH_COMPLETE))
    {
      FLASHStatus = FLASH_ProgramHalfWord(Address, Data);
      Address = Address + 2;
    }

    /* Check the correctness of written data */
    Address = BANK1_WRITE_START_ADDR;

    while((Address < BANK1_WRITE_END_ADDR) && (MemoryProgramStatus != FAILED))
    {
      if((*(__IO uint16_t*) Address) != Data)
      {
        MemoryProgramStatus = FAILED;
      }
      Address += 2;
    }
  }
  else
  { 
    /* Error to program the flash : The desired pages are write protected */ 
    MemoryProgramStatus = FAILED;
  }
#endif /* FLASH_PAGE_PROGRAM */

  while (1)
  {
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

