#ifndef __SYS_WATCHDOG_H__
#define __SYS_WATCHDOG_H__


#define USE_IWDG										//定义使能独立看门狗
#define IWDG_Prescale				0x04		//独立看门狗预分频因子  分频数=1<<(IWDG_Prescale+2)
#define IWDG_Reset_Time			100		//独立看门狗超时时间ms


void sys_watchDogInit(void);
void sys_watchDogReset(void);

#endif
