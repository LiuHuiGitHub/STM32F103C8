#include "stm32f10x.h"
#include "sys_key.h"


/*按键初始化*/
void sys_keyInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//@配置外部中断必须开启AFIO时钟
    /**
    @code
      =========================================================================================
        NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority
      =========================================================================================
       NVIC_PriorityGroup_0  |                0                  |            0-15
      -----------------------------------------------------------------------------------------
    	 NVIC_PriorityGroup_1  |                0-1                |            0-7
      -----------------------------------------------------------------------------------------
       NVIC_PriorityGroup_2  |                0-3                |            0-3
      -----------------------------------------------------------------------------------------
       NVIC_PriorityGroup_3  |                0-7                |            0-1
      -----------------------------------------------------------------------------------------
       NVIC_PriorityGroup_4  |                0-15               |            0
      =========================================================================================
    @endcode
    */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//中断源 外部中断线0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//中断抢占优先权
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//中断从优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能中断
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//中断源 外部中断线1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//中断源 外部中断线2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//中断源 外部中断线9-5
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//中断源 外部中断线15-10
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//选择PA0作外部中断线路
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
    EXTI_ClearFlag(EXTI_Line0 | EXTI_Line1 | EXTI_Line2);//清除外部中断标志
    EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//设置EXTI线路为中断请求
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//设置EXTI线路为中断请求
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发
    EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}
