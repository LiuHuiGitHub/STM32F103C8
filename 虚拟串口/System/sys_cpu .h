#include "C8051F040.h"

void sys_taskWatchDogInit(void)
{
    SFRPAGE = 0x00;
    RSTSRC |= 1 << 2;           //使能时钟丢失检测
    WDTCN = 0xA5;               //使能看门狗定时器
}
void sys_taskWatchDog(void)
{
    char SFRPAGE_SAVE = SFRPAGE;
    SFRPAGE = 0x00;
    WDTCN = 0xA5;       //喂狗  超时时间47ms
    SFRPAGE = SFRPAGE_SAVE;
}
