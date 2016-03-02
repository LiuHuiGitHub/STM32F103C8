/*
================================================================================
*	  串口示波器驱动模块
*	  文件：Output_data.c
*	  描述：通过串口将数据发送至串口示波器
*     说明：需添加串口通信模块USART.c
================================================================================
*/
#include "Output_data.h"

extern void sys_usartSendData(unsigned char *Data, unsigned int SendSize);
/*
================================================================================
* 名称        : OutPut_Data()
* 描述        : 将数组的值发送给串口示波器显示
* 输入        : signed short OutData[4]
* 输出        : 无
* 说明        : 无
================================================================================
*/


void OutPut_Data(short int *OutData)
{
    unsigned char databuf[10] = {0};
    unsigned char i, j;
    unsigned short CRC_Temp = 0xffff;
    for(i = 0; i < 4; i++)
    {
        databuf[i * 2] = (unsigned char)(OutData[i] & 0xff);
        databuf[i * 2 + 1] = (unsigned char)(OutData[i] >> 8);
    }
    for(i = 0; i < 8; i++)
    {
        CRC_Temp ^= databuf[i];
        for(j = 0; j < 8; j++)
        {
            if(CRC_Temp & 0x01)
            {
                CRC_Temp = (CRC_Temp >> 1) ^ 0xa001;
            }
            else
            {
                CRC_Temp = CRC_Temp >> 1;
            }
        }
    }
    databuf[8] = CRC_Temp & 0xff;
    databuf[9] = CRC_Temp >> 8;
    sys_usartSendData(databuf, 10);
}
