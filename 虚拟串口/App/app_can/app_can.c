#include "app_can.h"
#include "CanMsg.h"
#include "app_canItf.h"
#include "uLib_queue.h"

UINT16 u16_MaxSendTime = 0;

#define SEND_QUEUE_LEN         ((UINT8)30)
LIB_QUEUE q_CanMsg;
ID_ENUM CanMsgQueue[SEND_QUEUE_LEN];


void app_canInit(void)
{
    UINT8 i;
    app_canItfInit();
    uLib_q_Create(&q_CanMsg, CanMsgQueue, sizeof(*CanMsgQueue), sizeof(CanMsgQueue) / sizeof(*CanMsgQueue));
    for(i = 0; i < NUM_OF_ID_NUM; i++)
    {
        if(s_CanMsgBuff[i].Time > u16_MaxSendTime)
        {
            u16_MaxSendTime = s_CanMsgBuff[i].Time;
        }
    }
}
static void app_canHandler10ms(void)
{
    static UINT16 u16_count = 0;
    UINT8 index;
    for(index = 0; index < NUM_OF_ID_NUM; index++)
    {
        if(u16_count % (s_CanMsgBuff[index].Time / 10) == 0)
        {
            uLib_q_Insert(&q_CanMsg, &index);
        }
    }
    u16_count++;
    if(u16_count >= u16_MaxSendTime)
        u16_count = 0;
}

void app_canHandler1ms(void)
{
    static UINT8 u8_count = 0;
    UINT8 i;
    void *p;
    for(i = 0; i < 3; i++)    //只有3个发送缓冲区，每次最多发送三个
    {
        p = uLib_q_First(&q_CanMsg);
        if(p == NULL)
        {
            break;
        }
        else
        {
            app_canItfSendMsg(&s_CanMsgBuff[*(ID_ENUM *)p]);
            uLib_q_Delete(&q_CanMsg);
        }
    }
    if(++u8_count >= 10)
    {
        u8_count = 0;
        app_canHandler10ms();
    }
}

void app_canLight(Light_ENUM light, BOOL sw)
{
    UINT8 Index, i;
    Index = e_Light_To_Id[light];
    for(i = 0; i < 8; i++)
    {
        s_CanMsgBuff[Index].Data[i] &= s_Light[light].Data[0][i];
        if(sw)
            s_CanMsgBuff[Index].Data[i] |= s_Light[light].Data[2][i];
        else
            s_CanMsgBuff[Index].Data[i] |= s_Light[light].Data[1][i];
    }
}

void app_canCircle(Circle_ENUM Circle, UINT8 angle)
{
    UINT8 Index, i;
    Index = e_Circle_To_Id[Circle];
    if(angle > MAX_CIRCLE_ANGLE)
        angle = MAX_CIRCLE_ANGLE;
    for(i = 0; i < 8; i++)
    {
        s_CanMsgBuff[Index].Data[i] &= s_Circle[Circle].Data[0][i];
        s_CanMsgBuff[Index].Data[i] |= s_Circle[Circle].Data[angle + 1][i];
    }
}
