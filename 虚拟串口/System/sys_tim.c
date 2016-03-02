/******************************************************************************

  Copyright (C), 2001-2011, DCN Co., Ltd.

 ******************************************************************************
  File Name     : sys_tim.c
  Version       : Initial Draft
  Author        : Hui.Liu
  Created       : 2015/4/21
  Last Modified :
  Description   : sys_tim.c
  Function List :
  History       :
  1.Date        : 2015/4/21
    Author      : Hui.Liu
    Modification: Created file

******************************************************************************/

#include "stm32f10x.h"
#include "sys_tim.h"

#define ENTER_INTERRUPT()
#define EXIT_INTERRUPT()

typedef struct
{
    TIM_TypeDef *tim;
    uint32_t    RCC_Clk;
    uint16_t    IRQn;
    uint8_t     PreemptionPriority;
    uint8_t     SubPriority;
} TIM_STRUCT;


void (*TIM_Interrupt[4])(void) =
{
    (void *)0,
    (void *)0,
    (void *)0,
    (void *)0,
};

const TIM_STRUCT TIMx[4] =
{
    TIM1,   RCC_APB2Periph_TIM1,    TIM1_UP_IRQn,   TIM1_PreemptionPriority,    TIM1_SubPriority,
    TIM2,   RCC_APB1Periph_TIM2,    TIM2_IRQn,      TIM2_PreemptionPriority,    TIM2_SubPriority,
    TIM3,   RCC_APB1Periph_TIM3,    TIM3_IRQn,      TIM3_PreemptionPriority,    TIM3_SubPriority,
    TIM4,   RCC_APB1Periph_TIM4,    TIM4_IRQn,      TIM4_PreemptionPriority,    TIM4_SubPriority,
};

void sys_timInit(TIM_ENUM TIM_x, uint16_t Prescaler, uint16_t Period, void(*CallBack)(void))
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    if(TIMx[TIM_x].tim == TIM1)
    {
        RCC_APB2PeriphClockCmd(TIMx[TIM_x].RCC_Clk, ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(TIMx[TIM_x].RCC_Clk, ENABLE);
    }
    TIM_TimeBaseInitStruct.TIM_Prescaler = Prescaler ? Prescaler - 1 : 0;
    TIM_TimeBaseInitStruct.TIM_Period = Period ? Period - 1 : 0;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIMx[TIM_x].tim, &TIM_TimeBaseInitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = TIMx[TIM_x].IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = TIMx[TIM_x].PreemptionPriority;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = TIMx[TIM_x].SubPriority;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    TIM_Interrupt[TIM_x] = CallBack;

    TIM_ITConfig(TIMx[TIM_x].tim , TIM_IT_Update, ENABLE);

    TIM_Cmd(TIMx[TIM_x].tim, DISABLE);
}

void sys_timOpen(TIM_ENUM TIM_x)
{
    TIM_ClearFlag(TIMx[TIM_x].tim, TIM_IT_Update);
    TIM_Cmd(TIMx[TIM_x].tim, ENABLE);
}
void sys_timClose(TIM_ENUM TIM_x)
{
    TIM_Cmd(TIMx[TIM_x].tim, DISABLE);
}

void TIM_UpdateHandler(TIM_ENUM TIM_x)
{
    if(TIM_GetITStatus(TIMx[TIM_x].tim, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIMx[TIM_x].tim, TIM_IT_Update);
        if(TIM_Interrupt[TIM_x] != (void *)0)
        {
            (*TIM_Interrupt[TIM_x])();
        }
    }
}
#ifdef TIM_1_ENABLE
void TIM1_UP_IRQHandler(void)
{
    ENTER_INTERRUPT();
    TIM_UpdateHandler(TIM_1);
    EXIT_INTERRUPT();
}
#endif

#ifdef TIM_2_ENABLE
void TIM2_IRQHandler(void)
{
    ENTER_INTERRUPT();
    TIM_UpdateHandler(TIM_2);
    EXIT_INTERRUPT();
}
#endif

#ifdef TIM_3_ENABLE
void TIM3_IRQHandler(void)
{
    ENTER_INTERRUPT();
    TIM_UpdateHandler(TIM_3);
    EXIT_INTERRUPT();
}
#endif

#ifdef TIM_4_ENABLE
void TIM4_IRQHandler(void)
{
    ENTER_INTERRUPT();
    TIM_UpdateHandler(TIM_4);
    EXIT_INTERRUPT();
}
#endif
