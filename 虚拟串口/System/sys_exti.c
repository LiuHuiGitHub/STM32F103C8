/******************************************************************************

  Copyright (C), 2001-2011, DCN Co., Ltd.

 ******************************************************************************
  File Name     : sys_exti.c
  Version       : Initial Draft
  Author        : Hui.Liu
  Created       : 2015/4/21
  Last Modified :
  Description   : sys_exti.c
  Function List :
  History       :
  1.Date        : 2015/4/21
    Author      : Hui.Liu
    Modification: Created file

******************************************************************************/
#include "stm32f10x.h"
#include "sys_exti.h"

#define ENTER_INTERRUPT()
#define EXIT_INTERRUPT()

typedef struct
{
    IRQn_Type Channel;
    uint8_t PreemptionPriority;
    uint8_t SubPriority;
} EXTI_PRIO_STRUCT;

const EXTI_PRIO_STRUCT c_s_EXTI_PRIO[7] =
{
    EXTI0_IRQn,     EXTI0_PreemptionPriority,       EXTI0_SubPriority,
    EXTI1_IRQn,     EXTI1_PreemptionPriority,       EXTI1_SubPriority,
    EXTI2_IRQn,     EXTI2_PreemptionPriority,       EXTI2_SubPriority,
    EXTI3_IRQn,     EXTI3_PreemptionPriority,       EXTI3_SubPriority,
    EXTI4_IRQn,     EXTI4_PreemptionPriority,       EXTI4_SubPriority,
    EXTI9_5_IRQn,   EXTI9_5_PreemptionPriority,     EXTI9_5_SubPriority,
    EXTI15_10_IRQn, EXTI15_10_PreemptionPriority,   EXTI15_10_SubPriority,
};

typedef struct
{
    uint16_t Pin;
    uint8_t Index;
    uint8_t Source;
    uint32_t Line;
} EXTI_PIN_STRUCT;
const EXTI_PIN_STRUCT c_s_EXTI_PIN[NUM_OF_EXTI_PIN] =
{
    GPIO_Pin_0,     0,      GPIO_PinSource0,    EXTI_Line0,
    GPIO_Pin_1,     1,      GPIO_PinSource1,    EXTI_Line1,
    GPIO_Pin_2,     2,      GPIO_PinSource2,    EXTI_Line2,
    GPIO_Pin_3,     3,      GPIO_PinSource3,    EXTI_Line3,
    GPIO_Pin_4,     4,      GPIO_PinSource4,    EXTI_Line4,
    GPIO_Pin_5,     5,      GPIO_PinSource5,    EXTI_Line5,
    GPIO_Pin_6,     5,      GPIO_PinSource6,    EXTI_Line6,
    GPIO_Pin_7,     5,      GPIO_PinSource7,    EXTI_Line7,
    GPIO_Pin_8,     5,      GPIO_PinSource8,    EXTI_Line8,
    GPIO_Pin_9,     5,      GPIO_PinSource9,    EXTI_Line9,
    GPIO_Pin_10,    6,      GPIO_PinSource10,   EXTI_Line10,
    GPIO_Pin_11,    6,      GPIO_PinSource11,   EXTI_Line11,
    GPIO_Pin_12,    6,      GPIO_PinSource12,   EXTI_Line12,
    GPIO_Pin_13,    6,      GPIO_PinSource13,   EXTI_Line13,
    GPIO_Pin_14,    6,      GPIO_PinSource14,   EXTI_Line14,
    GPIO_Pin_15,    6,      GPIO_PinSource15,   EXTI_Line15,
};

typedef struct
{
    GPIO_TypeDef *Port;
    uint32_t Periph;
    uint8_t PortSource;
} EXTI_PORT_STRUCT;
const EXTI_PORT_STRUCT c_s_EXTI_PORT[NUM_OF_EXTI_PORT] =
{
    GPIOA,  RCC_APB2Periph_GPIOA,   GPIO_PortSourceGPIOA,
    GPIOB,  RCC_APB2Periph_GPIOB,   GPIO_PortSourceGPIOB,
    GPIOC,  RCC_APB2Periph_GPIOC,   GPIO_PortSourceGPIOC,
    GPIOD,  RCC_APB2Periph_GPIOD,   GPIO_PortSourceGPIOD,
    GPIOE,  RCC_APB2Periph_GPIOE,   GPIO_PortSourceGPIOE,
    GPIOF,  RCC_APB2Periph_GPIOF,   GPIO_PortSourceGPIOF,
    GPIOG,  RCC_APB2Periph_GPIOG,   GPIO_PortSourceGPIOG,
};

void (*ExtiBackCallFun[NUM_OF_EXTI_PIN])(void) =
{
    (void *)0,
    (void *)0,
    (void *)0,
    (void *)0,
    (void *)0,
    (void *)0,
    (void *)0,
    (void *)0,
    (void *)0,
    (void *)0,
    (void *)0,
    (void *)0,
    (void *)0,
    (void *)0,
    (void *)0,
    (void *)0,
};
/*****************************************************************************
 Prototype    : sys_extiInit
 Description  :
 Input        : EXTI_PORT_ENUM EXTI_PORTx,
                EXTI_PIN_ENUM EXTI_Pin_x,
                EXTITrigger_TypeDef Trigger,
                void(*pBackCallFun)(void)

 Output       : None
 Return Value : void

  History        :
  1.Date         : 2015/4/21
    Author       : Hui.Liu
    Modification : Created function
*****************************************************************************/
void sys_extiInit(EXTI_PORT_ENUM EXTI_PORTx, EXTI_PIN_ENUM EXTI_Pin_x, EXTITrigger_TypeDef Trigger, void(*pBackCallFun)(void))
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    ExtiBackCallFun[EXTI_Pin_x] = pBackCallFun;

    RCC_APB2PeriphClockCmd(c_s_EXTI_PORT[EXTI_PORTx].Periph, ENABLE);

    GPIO_InitStructure.GPIO_Pin = c_s_EXTI_PIN[EXTI_Pin_x].Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(c_s_EXTI_PORT[EXTI_PORTx].Port, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);                                        //@配置外部中断必须开启AFIO时钟

    NVIC_InitStructure.NVIC_IRQChannel = c_s_EXTI_PRIO[c_s_EXTI_PIN[EXTI_Pin_x].Index].Channel; //@中断源 外部中断线
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = c_s_EXTI_PRIO[c_s_EXTI_PIN[EXTI_Pin_x].Index].PreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = c_s_EXTI_PRIO[c_s_EXTI_PIN[EXTI_Pin_x].Index].SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_EXTILineConfig(c_s_EXTI_PORT[EXTI_PORTx].PortSource, c_s_EXTI_PIN[EXTI_Pin_x].Source); //@绑定外部中断端口

    EXTI_ClearFlag(c_s_EXTI_PIN[EXTI_Pin_x].Line);                                              //@清除外部中断标志

    EXTI_InitStructure.EXTI_Line = c_s_EXTI_PIN[EXTI_Pin_x].Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                                         //@设置EXTI线路为中断请求
    EXTI_InitStructure.EXTI_Trigger = Trigger;                                                  //@设置触发类型
    EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

void EXTI_Handler(EXTI_PIN_ENUM index)
{
    if(EXTI_GetITStatus(c_s_EXTI_PIN[index].Line) != RESET)
    {
        EXTI_ClearITPendingBit(c_s_EXTI_PIN[index].Line);
        if(ExtiBackCallFun != (void *)0)
        {
            (*ExtiBackCallFun[index])();
        }
    }
}

void EXTI0_IRQHandler(void)
{
    ENTER_INTERRUPT();
    EXTI_Handler(EXTI_PIN_0);
    EXIT_INTERRUPT();
}
void EXTI1_IRQHandler(void)
{
    ENTER_INTERRUPT();
    EXTI_Handler(EXTI_PIN_1);
    EXIT_INTERRUPT();
}
void EXTI2_IRQHandler(void)
{
    ENTER_INTERRUPT();
    EXTI_Handler(EXTI_PIN_2);
    EXIT_INTERRUPT();
}
void EXTI3_IRQHandler(void)
{
    ENTER_INTERRUPT();
    EXTI_Handler(EXTI_PIN_3);
    EXIT_INTERRUPT();
}
void EXTI4_IRQHandler(void)
{
    ENTER_INTERRUPT();
    EXTI_Handler(EXTI_PIN_4);
    EXIT_INTERRUPT();
}
void EXTI9_5_IRQHandler(void)
{
    ENTER_INTERRUPT();
    EXTI_Handler(EXTI_PIN_5);
    EXTI_Handler(EXTI_PIN_6);
    EXTI_Handler(EXTI_PIN_7);
    EXTI_Handler(EXTI_PIN_8);
    EXTI_Handler(EXTI_PIN_9);
    EXIT_INTERRUPT();
}
void EXTI15_10_IRQHandler(void)
{
    ENTER_INTERRUPT();
    EXTI_Handler(EXTI_PIN_10);
    EXTI_Handler(EXTI_PIN_11);
    EXTI_Handler(EXTI_PIN_12);
    EXTI_Handler(EXTI_PIN_13);
    EXTI_Handler(EXTI_PIN_14);
    EXTI_Handler(EXTI_PIN_15);
    EXIT_INTERRUPT();
}
