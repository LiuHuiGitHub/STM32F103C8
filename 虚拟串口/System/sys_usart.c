#include "stm32f10x.h"
#include "sys_usart.h"


static void USART1_DMA_Init(void);
static void USART1_NVIC_Init(void);

void sys_usartInit(USART_TypeDef *USART_x, u32 Boud)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    if(USART_x == USART1)
    {
#if (!USART1_REMAP)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
#else
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    }
    else if(USART_x == USART2)
    {
#if (!USART2_REMAP)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
#else
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        GPIO_SetBits(GPIOC, GPIO_Pin_8);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    }
    else if(USART_x == USART3)
    {
#if (!USART3_PARTIALREMAP)&&(!USART3_FULLREMAP)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
        GPIO_SetBits(GPIOD, GPIO_Pin_8);
#elif (USART3_PARTIALREMAP)&&(USART3_FULLREMAP)
#err "USART3_PARTIALREMAP and USART3_FULLREMAP == 1"
#elif (USART3_PARTIALREMAP)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
#elif (USART3_FULLREMAP)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    }
    USART_InitStructure.USART_BaudRate = Boud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART_x, &USART_InitStructure);
    USART_Cmd(USART_x, ENABLE);
    if(USART_x == USART1)
    {
        USART1_NVIC_Init();
        USART1_DMA_Init();
    }
}

const char SendBuff[] = "\r\nWelcome To Use STM32F103VET6!\r\n";

static void USART1_DMA_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SendBuff;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = sizeof(SendBuff) / sizeof(*SendBuff);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel4, ENABLE);
//	while(DMA_GetCurrDataCounter(DMA1_Channel4));
}
/*USARTx使用DMA方式发送数据*/
void sys_usartSendData(uint8_t *Data, uint32_t SendSize)
{
    while(DMA_GetCurrDataCounter(DMA1_Channel4));
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC4));
    DMA_ClearFlag(DMA1_FLAG_TC4);
    DMA1_Channel4->CCR &= 0xffffffe;
    DMA1_Channel4->CMAR = (uint32_t)Data;
    DMA1_Channel4->CNDTR = SendSize;
    DMA1_Channel4->CCR |= 0x00000001;
}

#if 1
/*USARTx发送重定义*/
#include "stdio.h"
int fputc(int ch, FILE *f)
{
    USART2->DR = (unsigned char)ch;
    while(!(USART2->SR & USART_FLAG_TXE));
    return (ch);
}
#endif

void sys_usartTransmit(USART_TypeDef *Usartx, uint8_t *ptr, uint16_t size)
{
    while(size--)
    {
        Usartx->DR = (unsigned char) * ptr;
        ptr++;
        while(!(Usartx->SR & USART_FLAG_TXE));
    }
}

static void USART1_NVIC_Init(void)
{
    NVIC_InitTypeDef NVICx;
    NVICx.NVIC_IRQChannel = USART1_IRQn;
    NVICx.NVIC_IRQChannelPreemptionPriority = 1;
    NVICx.NVIC_IRQChannelSubPriority = 0;
    NVICx.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVICx);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

//#define USE_UCOS_II
#ifdef USE_UCOS_II
extern void OSIntEnter(void);
extern void OSIntExit(void);
#else
#define OSIntEnter()
#define OSIntExit()
#endif

void drv_usart1TxHandler(void);
void drv_usart1RxHandler(void);

void USART1_IRQHandler(void)
{
    OSIntEnter();
    if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        drv_usart1RxHandler();
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
    OSIntExit();
}


