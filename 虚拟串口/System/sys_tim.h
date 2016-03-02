/******************************************************************************

  Copyright (C), 2001-2011, DCN Co., Ltd.

 ******************************************************************************
  File Name     : sys_tim.h
  Version       : Initial Draft
  Author        : Hui.Liu
  Created       : 2015/4/21
  Last Modified :
  Description   : sys_tim.c header file
  Function List :
  History       :
  1.Date        : 2015/4/21
    Author      : Hui.Liu
    Modification: Created file

******************************************************************************/

#ifndef __SYS_TIM_H__
#define __SYS_TIM_H__

/*示例：

void LED1(void)
{
    LED_1 = !LED_1;
}
int main(void)
{
    sys_timInit(TIM_4, 72, 1000, LED1);
    sys_timOpen(TIM_4);
    while(1)
    {

    }
	return 0;
}
*/

typedef enum
{
    TIM_1 = 0,
    TIM_2,
    TIM_3,
    TIM_4,
} TIM_ENUM;

/*TIM 使能配置*/
#define TIM_1_ENABLE
//#define TIM_2_ENABLE
//#define TIM_3_ENABLE
#define TIM_4_ENABLE

/*TIM 优先级配置*/
#define TIM1_PreemptionPriority        (uint8_t)1
#define TIM1_SubPriority               (uint8_t)1

#define TIM2_PreemptionPriority        (uint8_t)1
#define TIM2_SubPriority               (uint8_t)1

#define TIM3_PreemptionPriority        (uint8_t)1
#define TIM3_SubPriority               (uint8_t)1

#define TIM4_PreemptionPriority        (uint8_t)1
#define TIM4_SubPriority               (uint8_t)1

void sys_timInit(TIM_ENUM TIM_x, uint16_t Prescaler, uint16_t Period, void(*CallBack)(void));
void sys_timOpen(TIM_ENUM TIM_x);
void sys_timClose(TIM_ENUM TIM_x);

#endif
