/*
 * main.c
 *
 *  Created on: 07.02.2013
 *      Author: Filatov Grigorij
 */

//=============================================================================
// ����������� ����������� ����� �� ����������
//=============================================================================
 #include "stm32f10x.h"
 #include <stm32f10x_gpio.h>
 #include <stm32f10x_rcc.h>
 #include <stm32f10x_tim.h>

 #include <sin_mas.h>
 #include <misc.h>
 #include <stm32f10x_conf.h>
//================================================================================
// ������������� ���������.
//=================================================================================
void init_gpio() {

	//������� ���� C
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);//������������ ���-��� �����!!!!!
	GPIO_InitTypeDef    port_init;
	port_init.GPIO_Pin = GPIO_Pin_8; //�������� ������ PC8
	port_init.GPIO_Speed = GPIO_Speed_50MHz; //������������ �������� ������
	port_init.GPIO_Mode = GPIO_Mode_AF_PP; //�����
	GPIO_Init(GPIOC, &port_init); //������� �������� ��������� � �������� �����
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);//����������� �������3 � ������ pc8 and pc9
	//-----------------------------------------------------------------------------------

	//������� ���� C
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
	//GPIO_InitTypeDef    port_init2;
	//port_init2.GPIO_Pin = GPIO_Pin_9; //�������� ������ PC9
	//port_init2.GPIO_Speed = GPIO_Speed_50MHz; //������������ �������� ������
	//port_init2.GPIO_Mode = GPIO_Mode_AF_PP; //�����
	//GPIO_Init(GPIOC, &port_init2); //������� �������� ��������� � �������� �����
	//GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
}

//===================================================================================
//������������� ����������
//====================================================================================
void init_it()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitTypeDef 	NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig((TIM_TypeDef *)TIM3_BASE,TIM_IT_Update, ENABLE);
	TIM_ClearITPendingBit((TIM_TypeDef *)TIM3_BASE, TIM_IT_Update);
}
//======================================================================================
void tim_init(){

	//�������� ������ 3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    TIM_TimeBaseInitTypeDef base_timer;
    TIM_TimeBaseStructInit(&base_timer);

    base_timer.TIM_Prescaler = 240;   // �������� �������
    base_timer.TIM_Period = 10001;  // ������
    base_timer.TIM_CounterMode = TIM_CounterMode_Up  ; // ����������� �� ������//VVERH
    TIM_TimeBaseInit(TIM3, &base_timer);

    TIM_OCInitTypeDef tim_init;
    TIM_OCStructInit(&tim_init);
    tim_init.TIM_OCMode = TIM_OCMode_PWM1;   // �������� � ������ ��� ( PWM )
    tim_init.TIM_OutputState = TIM_OutputState_Enable;//  �������� ��������� ������
    tim_init.TIM_Pulse = 50;//������ �������� �������� ��� �������� � Capture
    tim_init.TIM_OCPolarity = TIM_OCPolarity_High;  // ������������� ����������
    TIM_OC3Init(TIM3,&tim_init);

    //---------------------------------------------------------------------------------
    // ��� �������� ����������------------------------------------------------------
    //TIM_OCInitTypeDef tim_init2;
        //TIM_OCStructInit(&tim_init2);
        //tim_init2.TIM_OCMode = TIM_OCMode_PWM1;   // �������� � ������ ��� ( PWM )
        //tim_init2.TIM_OutputState = TIM_OutputState_Enable;//  �������� ��������� ������
        //tim_init2.TIM_Pulse = 500;//������ �������� �������� ��� �������� � Capture
        //tim_init2.TIM_OCPolarity = TIM_OCPolarity_High;  // ������������� ����������
    //-----------------------------------------------------------------------------------


    TIM_OC3Init(TIM3,&tim_init);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);//����� led (pc8,pin8)

    //TIM_OC4Init(TIM3,&tim_init2);
    //TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);//������� led (pc9,pin9)


    TIM_ARRPreloadConfig(TIM3,ENABLE);

    // interrupt config
    init_it();// �������� �� �������

    TIM_Cmd(TIM3,ENABLE);   // ��������� ����
}

//=========================================================================================================
//��������� ����������
//=======================================================================================================
//������, �������

/*int occ_cnt[64] = {
		0,100,200,300,400,500,600,700,800,900,1000,
		1100,1200,1300,1400,1500,1600,1700,1800,1900,2000,
		2100,2200,2300,2400,2500,2600,2700,2800,2900,3000,
		3100,3200,3300,3400,3500,3600,3700,3800,3900,4000,
        4100,4200,4300,4500,4600,4700,4800,4900,5000,5100,
        5200,5300,5400,5500,5600,5700,5800,5900,6000,6100,6200,6300};
int i=0;*/

//------------------------------------------------------------------------------------------------
//int occ_cnt = 0;
void TIM3_IRQHandler ()
{
	switch (TIM_GetITStatus_IT(TIM3, TIM_IT_Update))
	{
	case 0:
		break;
	default:
	{
		/*Set the Capture Compare Register value*/
		switch(i/64)
		{
		case 0:
			TIM3->CCR3 = occ_cnt[i];
			i+=1;
			break;
		default:
			switch(i / 128)
			{
			case 0:
				//TIM3->CCR3 = 0;
				i+=1;
				break;
			default:
				//TIM3->CCR3 = 0;
				i = 0;
				break;
			}
			break;
		}
		//TIM3->CCR4 = occ_cnt;
//		switch (i)
//		{
//		case 64:
//			i=0;
//		break;
//		default:
//			break;
//		}
		//GPIOC->ODR^=GPIO_Pin_9;
		//GPIOC->ODR^=GPIO_Pin_8;
	}
	TIM_ClearITPendingBit_M(TIM3, TIM_IT_Update);
	//GPIOC->ODR^=GPIO_Pin_8;
	}
}
//==============================================================================
//������� ������� ������ ���� ����� � MAIN, �������� ���������
//=================================================================================
int main(void){
    init_gpio();
    tim_init();

    while (1){
    }

    return 0;
}
//=======================================================================================



















