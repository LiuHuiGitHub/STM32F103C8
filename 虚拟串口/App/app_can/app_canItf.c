#include "stm32f10x.h"
#include "sys_can.h"
#include "CanMsg.h"

void app_canItfInit(void)
{
    sys_canInit();
    CAN_TxMessage.IDE = 0;
    CAN_TxMessage.RTR = 0;
    CAN_TxMessage.DLC = 8;
}

void app_canItfSendMsg(CanMsg_STRUCT *msg)
{
    CAN_TxMessage.StdId = msg->Id;
    CAN_TxMessage.Data[0] = msg->Data[0];
    CAN_TxMessage.Data[1] = msg->Data[1];
    CAN_TxMessage.Data[2] = msg->Data[2];
    CAN_TxMessage.Data[3] = msg->Data[3];
    CAN_TxMessage.Data[4] = msg->Data[4];
    CAN_TxMessage.Data[5] = msg->Data[5];
    CAN_TxMessage.Data[6] = msg->Data[6];
    CAN_TxMessage.Data[7] = msg->Data[7];
    CAN_Transmit(CAN1, &CAN_TxMessage);
}
