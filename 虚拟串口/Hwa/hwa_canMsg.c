#include "system.h"
#include "stm32f10x.h"
#include "hwa_canMsg.h"

STCan_Msg  CanMsgBuff;

STCan_Msg ListHead;
const STCan_Msg *pstListHead = &ListHead;
STCan_Msg *pstListWritePoint = NULL;
STCan_Msg *pstListReadPoint = NULL;

uint8_t u8_canMsgRxEN;

void hwa_canMsgInit(STCan_Msg *msg)
{
//	uint8_t i, len;
//	uint8_t *p = (uint8_t*)&msg;
//	len = sizeof(STCan_Msg);
//	for(i=0; i<len; i++)
//		*p = 0x00;
    msg->Msg.Rx.StdId = 0x00000000;
    msg->Msg.Rx.ExtId = 0x00000000;
    msg->Msg.Rx.IDE = 0x00;
    msg->Msg.Rx.RTR = 0x00;
    msg->Msg.Rx.DLC = 0x00;
    msg->Msg.Rx.Data[0] = 0x00;
    msg->Msg.Rx.Data[1] = 0x00;
    msg->Msg.Rx.Data[2] = 0x00;
    msg->Msg.Rx.Data[3] = 0x00;
    msg->Msg.Rx.Data[4] = 0x00;
    msg->Msg.Rx.Data[5] = 0x00;
    msg->Msg.Rx.Data[6] = 0x00;
    msg->Msg.Rx.Data[7] = 0x00;
    msg->Msg.Rx.FMI = 0x00;
    msg->Cycle = 0x00;
    msg->pstNextList = NULL;
}
/*******************************************************************************
* function:	APP_CanMsg_Handler
* explain:
* input:		none
*
* output:		none
* return:		none
* other:		none
*******************************************************************************/
void APP_CanMsg_Handler(void)
{
    STCan_Msg *pstMsg;
    if(u8_canMsgRxEN == 1)
    {
        if(pstListWritePoint != NULL)
        {
            pstMsg = malloc(sizeof(STCan_Msg));
            if(pstMsg != NULL)
            {
                hwa_canMsgInit(pstMsg);
                CAN_Receive(CAN1, CAN_FIFO0, &pstMsg->Msg.Rx);
                pstListWritePoint->pstNextList = pstMsg;
                pstListWritePoint = pstMsg;
            }
            else
            {
                u8_canMsgRxEN = 1;
                LED2 = 1;
            }
        }
        else
        {
            pstListWritePoint = &ListHead;
            hwa_canMsgInit(pstListWritePoint);
            CAN_Receive(CAN1, CAN_FIFO0, &pstListWritePoint->Msg.Rx);
        }
    }
}

void ListDelete(STCan_Msg *msg)
{
    STCan_Msg *p = msg;
    p = p;
}

void hwa_canSendMsg(void)
{
    pstListReadPoint = &ListHead;
    while(pstListReadPoint != NULL)
    {
        CAN_Transmit(CAN1, &pstListReadPoint->Msg.Tx);
        Delay_ms(1);
        pstListReadPoint = pstListReadPoint->pstNextList;
    }
}

