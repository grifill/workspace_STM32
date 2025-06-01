#include "sys_init.h"
#include "stm32f3_discovery.h"


int main()
{
  SysTick_Config(720000);
  clk_init();
  led_init();
  
  while(1)
  {
   
  }
}
