#ifndef __HWA_CANMSG_H__
#define __HWA_CANMSG_H__


struct STCAN_Msg_
{
    union
    {
        CanRxMsg Rx;
        CanTxMsg Tx;
    } Msg;
    uint8_t Cycle;
    struct STCAN_Msg_ *pstNextList;
};

typedef struct STCAN_Msg_   STCan_Msg;

extern uint8_t u8_canMsgRxEN;
extern STCan_Msg  CanMsgBuff;

void hwa_canMsgInit(STCan_Msg *msg);
void hwa_canSendMsg(void);

#endif
