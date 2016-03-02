#include "stm32f10x.h"
#include "sys_pwm.h"

static UINT16 u16_TimerPeriod;

static void TIM1_Init(UINT32 freq)
{
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    if(freq >= (1100))
    {
        u16_TimerPeriod = (SystemCoreClock / freq - 1);
        TIM_TimeBaseInitStructure.TIM_Period = u16_TimerPeriod;
        TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
    }
    else if(freq >= 110)
    {
        u16_TimerPeriod = (SystemCoreClock / freq / 10 - 1);
        TIM_TimeBaseInitStructure.TIM_Period = u16_TimerPeriod;
        TIM_TimeBaseInitStructure.TIM_Prescaler = 10 - 1;
    }
    else if(freq >= 11)
    {
        u16_TimerPeriod = (SystemCoreClock / freq / 100 - 1);
        TIM_TimeBaseInitStructure.TIM_Period = u16_TimerPeriod;
        TIM_TimeBaseInitStructure.TIM_Prescaler = 100 - 1;
    }
    else
    {
        u16_TimerPeriod = (SystemCoreClock / freq / 2000 - 1);
        TIM_TimeBaseInitStructure.TIM_Period = u16_TimerPeriod;
        TIM_TimeBaseInitStructure.TIM_Prescaler = 2000 - 1;
    }
    TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;

#ifdef ENABLE_OC1
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
#endif
#ifdef ENABLE_OC2
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
#endif
#ifdef ENABLE_OC3
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
#endif
#ifdef ENABLE_OC4
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
#endif
}

void sys_pwmInit(UINT32 freq)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    UINT16 GPIO_Pin = 0x0000;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_TIM1, ENABLE);
#ifdef ENABLE_OC1
    GPIO_Pin |= GPIO_Pin_8;
#endif
#ifdef ENABLE_OC2
    GPIO_Pin |= GPIO_Pin_9;
#endif
#ifdef ENABLE_OC3
    GPIO_Pin |= GPIO_Pin_10;
#endif
#ifdef ENABLE_OC4
    GPIO_Pin |= GPIO_Pin_11;
#endif
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    TIM1_Init(freq);
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void sys_pwmSetRatio(UINT8 Channal, UINT8 ratio)
{
    UINT16 temp;
    if(ratio > 100)
    {
        ratio = 100;
    }
    temp = (UINT16)(((UINT32) ratio * u16_TimerPeriod) / 100);
    if(ratio == 100)
    {
        temp = 0xffff;
    }
    switch(Channal)
    {
        case 0x01:
            TIM1->CCR1 = temp;
            break;
        case 0x02:
            TIM1->CCR2 = temp;
            break;
        case 0x03:
            TIM1->CCR3 = temp;
            break;
        case 0x04:
            TIM1->CCR4 = temp;
            break;
        default :
            return;
    }
}
