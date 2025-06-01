/**
  ******************************************************************************
  * @file    sys_init.h
  * @author  Filatov Grigoriy
  * @date    29-April-2017
  ******************************************************************************
  *
  * Copyright(C) Grifill, 2017
  * All rights reserved
  * 
  ******************************************************************************
  */
#ifndef SYS_INIT_H_
#define SYS_INIT_H_

#include        <stdint.h>

#include        "stm32f30x_conf.h"
#include 	"stm32f30x.h"


void clk_init();
void led_init();
void gps_init();
void usart1_init();
void console_init();

void timer2_init();



// Define TRUE, FALSE and bool if not defined
typedef enum {FALSE = 0, TRUE = !FALSE} bool;



#endif //SYS_INIT_H_
