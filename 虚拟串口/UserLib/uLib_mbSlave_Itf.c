#include "uLib_mbSlave_Itf.h"
#include "uLib_mbSlave.h"
#include "sys_usart.h"
#include "drive_usart.h"

void uLib_mbSlaveItfInit(void)
{
	drv_usart1Init(9600);
}

void uLib_mbSlaveItfReceiveCtrl(BOOL state)
{
    if(state)
    {
    }
    else
    {
    }
}

void uLib_mbSlaveItfTransmitData(UINT8 *dat, UINT16 len)
{
	sys_usartSendData(dat, len);
}

void uLib_mbSlaveItfHandler(void)
{
    if(st_USART1_RX.flag)
    {
        uLib_mbReadReqADU(st_USART1_RX.buff, st_USART1_RX.size);
        st_USART1_RX.size = 0;
        st_USART1_RX.flag = FALSE;
    }
}

