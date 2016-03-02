#include "drive_usart.h"
#include "sys_usart.h"
#include "typedef.h"
#include "string.h"
#include "system.h"
#include "sys_usart.h"

USART_RX_STRUCT st_USART1_RX;

static void TIM3_OverTimeInit(UINT16 overtime);
static void TIM3_OverTimeReset(void);

void drv_usart1Init(UINT32 boud)
{
    st_USART1_RX.flag = FALSE;
    st_USART1_RX.size = 0;
    sys_usartInit(USART1, boud);
    TIM3_OverTimeInit((UINT16)(38500000 / boud));				//1000000us/boud*11(1 start, 8 data, 2 stop)*3.5
}
void drv_usart1TxHandler(void)
{
}
void drv_usart1RxHandler(void)
{
    if(st_USART1_RX.flag != FALSE)
    {
        st_USART1_RX.flag = FALSE;
        st_USART1_RX.size = 0;
    }
    if(st_USART1_RX.size < USART1_RxBuff_SIZE)
    {
        st_USART1_RX.buff[st_USART1_RX.size] = USART1->DR;
        st_USART1_RX.size++;
    }
    TIM3_OverTimeReset();						//open tim and reset tim cnt and clear rx success flag
}

static void drv_usartRx(void)
{
    st_USART1_RX.flag = TRUE;
}

void TIM3_OverTimeInit(UINT16 overtime)
{
    TIM_TimeBaseInitTypeDef TIMx;
    NVIC_InitTypeDef NVICx;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIMx.TIM_Period = overtime;
    TIMx.TIM_Prescaler = 72 - 1;
    TIMx.TIM_ClockDivision = 0;
    TIMx.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIMx);
    NVICx.NVIC_IRQChannel = TIM3_IRQn;
    NVICx.NVIC_IRQChannelPreemptionPriority = 1;
    NVICx.NVIC_IRQChannelSubPriority = 3;
    NVICx.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVICx);
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, DISABLE);
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		drv_usartRx();
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		TIM_Cmd(TIM3, DISABLE);
	}
}
void TIM3_OverTimeReset(void)
{
    TIM3->CNT = 0;
    st_USART1_RX.flag = FALSE;
    TIM_Cmd(TIM3, ENABLE);
}

