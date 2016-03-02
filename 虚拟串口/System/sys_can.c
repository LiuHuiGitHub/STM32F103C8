#include "stm32f10x.h"
#include "sys_can.h"

CanTxMsg CAN_TxMessage;
CanRxMsg CAN_RxMessage;

static void CAN_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
    /* Configure CAN pin: RX */									               // PB8
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* Configure CAN pin: TX */									               // PB9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void CAN_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void CAN_Mode_Config(void)
{
    CAN_InitTypeDef CAN_InitStructure;
    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);
    CAN_InitStructure.CAN_TTCM = DISABLE;			 //MCR-TTCM  关闭时间触发通信模式使能
    CAN_InitStructure.CAN_ABOM = ENABLE;			 //MCR-ABOM  自动离线管理
    CAN_InitStructure.CAN_AWUM = ENABLE;			 //MCR-AWUM  使用自动唤醒模式
    CAN_InitStructure.CAN_NART = DISABLE;			 //MCR-NART  禁止报文自动重传	  DISABLE-自动重传
    CAN_InitStructure.CAN_RFLM = DISABLE;			 //MCR-RFLM  接收FIFO 锁定模式  DISABLE-溢出时新报文会覆盖原有报文
    CAN_InitStructure.CAN_TXFP = DISABLE;			 //MCR-TXFP  发送FIFO优先级 DISABLE-优先级取决于报文标示符
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW = CAN_SJW_3tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
    CAN_InitStructure.CAN_Prescaler = 6;
    CAN_Init(CAN1, &CAN_InitStructure);
}

static void CAN_Filter_Config(void)
{
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;                     //工作在标识符掩码模式
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;                    //过滤器位宽为单个32位。
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x400 << 5;                              //要过滤的ID高位
    CAN_FilterInitStructure.CAN_FilterIdLow = CAN_RTR_Data | CAN_Id_Standard;           //要过滤的ID低位
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xffff;								//屏蔽0x400-0x4ff以外的所有ID
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xffff;								//屏蔽非标准数据帧
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0 ;				//过滤器被关联到FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;                              //使能过滤器
    CAN_FilterInit(&CAN_FilterInitStructure);
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

void sys_canInit(void)
{
    CAN_GPIO_Config();
    CAN_NVIC_Config();
    CAN_Mode_Config();
    CAN_Filter_Config();
}

extern CanRxMsg CAN_RxMessage;

void USB_LP_CAN1_RX0_IRQHandler(void)
{
    if(CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
        CAN_Receive(CAN1, CAN_FIFO0, &CAN_RxMessage);
    }
    do
    {
        CAN_FIFORelease(CAN1, CAN_FIFO0);
    }
    while(CAN_MessagePending(CAN1, CAN_FIFO0));
}

void CAN1_RX1_IRQHandler(void)
{
    CAN_FIFORelease(CAN1, CAN_FIFO1);
}

void CAN1_SCE_IRQHandler(void)
{
    if(CAN_GetITStatus(CAN1, CAN_IT_ERR) != RESET)
    {
        CAN_ClearITPendingBit(CAN1, CAN_IT_ERR);
    }
}

