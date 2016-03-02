/******************************************************************************
  Copyright (C), 2001-2011, DCN Co., Ltd.
 ******************************************************************************
  File Name     : sys_count.c
  Version       : Initial Draft
  Author        : hui.liu
  Created       : 2015/2/1
  Last Modified :
  Description   :
  Function List :
              sys_countAddCnt
              sys_countGetTime
              sys_countInit
              sys_countResetChannel
              TIM2_IRQHandler
  History       :
  1.Date        : 2015/2/1
    Author      : hui.liu
    Modification: Created file
******************************************************************************/

#include "stm32f10x.h"
#include "sys_count.h"
#include "typedef.h"

#define GET_TIME_CNT()				TIM2->CNT

static void sys_countAddCnt(void);
static void sys_count_Init(void);

void sys_countInit(void)
{
    TIM_TimeBaseInitTypeDef TIMx;
    NVIC_InitTypeDef NVICx;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIMx.TIM_Period = COUNT_OVER_TIME;										//65536*1000us
    TIMx.TIM_Prescaler = 72 - 1;
    TIMx.TIM_ClockDivision = 0;
    TIMx.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIMx);
    NVICx.NVIC_IRQChannel = TIM2_IRQn;
    NVICx.NVIC_IRQChannelPreemptionPriority = 0;
    NVICx.NVIC_IRQChannelSubPriority = 0;
    NVICx.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVICx);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    sys_count_Init();
}


//#define USE_UCOS_II
#ifdef USE_UCOS_II
extern void OSIntEnter(void);
extern void OSIntExit(void);
#else
#define OSIntEnter()
#define OSIntExit()
#endif

void TIM2_IRQHandler(void)			//65.536ms interrupt
{
    OSIntEnter();
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        sys_countAddCnt();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
    OSIntExit();
}



typedef struct _Counter_STRUCT
{
    UINT16 u16_LastCount;
    UINT16 u16_UpdateCnt;
} Counter_STRUCT;

volatile static Counter_STRUCT s_Counter[SYS_COUNT_CHANNEL_NUM] = {0};


static void sys_countAddCnt(void)
{
    register UINT8 i;
    for(i = 0; i < SYS_COUNT_CHANNEL_NUM; i++)
    {
        s_Counter[i].u16_UpdateCnt++;
    }
}

static void sys_count_Init(void)
{
    register UINT8 i;
    for(i = 0; i < SYS_COUNT_CHANNEL_NUM; i++)
    {
        s_Counter[i].u16_UpdateCnt = 0;
    }
}
/*****************************************************************************
 Prototype    : sys_countResetChannel
 Description  : init channel (0<=channle<=SYS_COUNT_CHANNEL_NUM)
 Input        : UINT8 channel
 Output       : None
 Return Value :

  History        :
  1.Date         : 2015/2/1
    Author       : hui.liu
    Modification : Created function
*****************************************************************************/
void sys_countResetChannel(UINT8 channel)
{
    s_Counter[channel].u16_LastCount = GET_TIME_CNT();
    s_Counter[channel].u16_UpdateCnt = 0;
}

/*****************************************************************************
 Prototype    : sys_countGetTime
 Description  : return time (unit us)
 Input        : UINT8 channel
 Output       : None
 Return Value :

  History        :
  1.Date         : 2015/2/1
    Author       : hui.liu
    Modification : Created function
*****************************************************************************/
UINT32 sys_countGetTime(register UINT8 channel)
{
    UINT32 temp = GET_TIME_CNT();
    if(s_Counter[channel].u16_UpdateCnt)
    {
        temp += s_Counter[channel].u16_UpdateCnt * (COUNT_OVER_TIME + 1);
    }
    temp -= s_Counter[channel].u16_LastCount;
    sys_countResetChannel(channel);
    return temp;
}
