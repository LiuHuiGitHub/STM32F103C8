#include "stm32f10x_it.h"
#include "system.h"

//#define USE_UCOS_II
#ifdef USE_UCOS_II
extern void OSIntEnter(void);
extern void OSIntExit(void);
#else
#define OSIntEnter()
#define OSIntExit()
#endif


void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    while(1)
    {
    }
}

void MemManage_Handler(void)
{
    while(1)
    {
    }
}

void BusFault_Handler(void)
{
    while(1)
    {
    }
}

void UsageFault_Handler(void)
{
    while(1)
    {
    }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}
void SysTick_Handler1ms(void);		//defined in main.c
void sys_taskScheduler(void);     //defined in sys_core.c
void SysTick_Handler(void)
{
    SysTick_Handler1ms();
    sys_taskScheduler();
}

