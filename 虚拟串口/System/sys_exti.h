/******************************************************************************

  Copyright (C), 2001-2011, DCN Co., Ltd.

 ******************************************************************************
  File Name     : sys_exti.h
  Version       : Initial Draft
  Author        : Hui.Liu
  Created       : 2015/4/21
  Last Modified :
  Description   : sys_exti.c header file
  Function List :
  History       :
  1.Date        : 2015/4/21
    Author      : Hui.Liu
    Modification: Created file

******************************************************************************/
#ifndef __SYS_EXTI_H__
#define __SYS_EXTI_H__
/*示例：
#include "stm32f10x.h"

void LED1(void)
{
    LED_1 = !LED1;
}
void LED2(void)
{
    LED_2 = !LED2;
}
int main(void)
{
    LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    sys_extiInit(EXTI_PORTB, EXTI_PIN_9, EXTI_Trigger_Rising, LED1);
    sys_extiInit(EXTI_PORTA, EXTI_PIN_1, EXTI_Trigger_Falling, LED2);
    while(1)
    {
    }
	return 0;
}
*/
typedef enum
{
    EXTI_PORTA = 0,
    EXTI_PORTB,
    EXTI_PORTC,
    EXTI_PORTD,
    EXTI_PORTE,
    EXTI_PORTF,
    EXTI_PORTG,
    NUM_OF_EXTI_PORT
} EXTI_PORT_ENUM;

typedef enum
{
    EXTI_PIN_0 = 0,
    EXTI_PIN_1,
    EXTI_PIN_2,
    EXTI_PIN_3,
    EXTI_PIN_4,
    EXTI_PIN_5,
    EXTI_PIN_6,
    EXTI_PIN_7,
    EXTI_PIN_8,
    EXTI_PIN_9,
    EXTI_PIN_10,
    EXTI_PIN_11,
    EXTI_PIN_12,
    EXTI_PIN_13,
    EXTI_PIN_14,
    EXTI_PIN_15,
    NUM_OF_EXTI_PIN
} EXTI_PIN_ENUM;

/*EXTI 优先级配置*/
#define EXTI0_PreemptionPriority        (uint8_t)1
#define EXTI0_SubPriority               (uint8_t)1

#define EXTI1_PreemptionPriority        (uint8_t)1
#define EXTI1_SubPriority               (uint8_t)1

#define EXTI2_PreemptionPriority        (uint8_t)1
#define EXTI2_SubPriority               (uint8_t)1

#define EXTI3_PreemptionPriority        (uint8_t)1
#define EXTI3_SubPriority               (uint8_t)1

#define EXTI4_PreemptionPriority        (uint8_t)1
#define EXTI4_SubPriority               (uint8_t)1

#define EXTI9_5_PreemptionPriority      (uint8_t)1
#define EXTI9_5_SubPriority             (uint8_t)1

#define EXTI15_10_PreemptionPriority    (uint8_t)1
#define EXTI15_10_SubPriority           (uint8_t)1

void sys_extiInit(EXTI_PORT_ENUM EXTI_PORTx, EXTI_PIN_ENUM EXTI_Pin_x, EXTITrigger_TypeDef Trigger, void(*pBackCallFun)(void));

#endif
