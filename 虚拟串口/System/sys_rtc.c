/*********************************************************************************
 * 文件名  ：sys_rtc.c
 * 描述    ：实时时钟模块
 * 库版本  ：ST3.5.0
 * 编写时间：2013年12月17日
 * 编写人  ：LiuHui
 *
 *
 * 修改时间：2014年4月5
 * 修改内容：
 *          @1加入TFT显示时间函数 uint32_t Time_Regulate(uint8_t x, uint16_t y);
 *          @2加入TFT触摸屏校时函数
 *          @3加入闹钟中断
 *
 * 修改时间：2014年4月7
 * 修改内容：
 *          @时间显示、时间设置移出RTC.c至NumClock.c
 *
**********************************************************************************/
#include "stm32f10x.h"
#include "sys_rtc.h"
#include "typedef.h"

volatile uint8_t b_rtcTimeDisplayFlag = 0;
volatile uint8_t b_rtcAlarmRingFlag = 0;

static void RTC_Configuration(void);
static void RTC_NVIC_Configuration(void);

void sys_rtcInit(void)
{
    RTC_Configuration();
    RTC_NVIC_Configuration();
//	/*设置闹钟*/
//	RTC_SetAlarm(0x09);
    /* Clear reset flags */
    RCC_ClearFlag();
}

/**-------------------------------------------------------
  * @函数名 RTC_Configuration
  * @功能   RTC实时时钟的参数配置函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
static void RTC_Configuration(void)
{
    /* PWR时钟（电源控制）与BKP时钟（RTC后备寄存器）使能 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    /*使能RTC和后备寄存器访问 */
    PWR_BackupAccessCmd(ENABLE);
//	/* 将外设BKP的全部寄存器重设为缺省值 */
//	BKP_DeInit();
    if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
    {
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
#if	RTC_CLK_LSE
        /* 使能LSE（外部32.768KHz低速晶振）*/
        RCC_LSEConfig(RCC_LSE_ON);
        /* 等待外部晶振震荡稳定输出 */
        while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
#elif RTC_CLK_LSI
        RCC_LSICmd(ENABLE);
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
#elif RTC_CLK_HSE128
        RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);
#else
#error not select RTC clock!
#endif
        /* 使能 RTC 的时钟供给 */
        RCC_RTCCLKCmd(ENABLE);
        /*等待RTC寄存器同步 */
        RTC_WaitForSynchro();
#if	RTC_CLK_LSE
        /* 32.768KHz晶振预分频值是32767,如果对精度要求很高可以修改此分频值来校准晶振 */
        RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
#elif RTC_CLK_LSI
        RTC_SetPrescaler(50000);
#elif RTC_CLK_HSE128
        RTC_SetPrescaler(62499);
#else
#error not select RTC clock!
#endif
        /* 等待上一次对RTC寄存器的写操作完成 */
        RTC_WaitForLastTask();
    }
    /*等待RTC寄存器同步 */
    RTC_WaitForSynchro();
    /* 等待上一次对RTC寄存器的写操作完成 */
    RTC_WaitForLastTask();
    /* 使能RTC的秒中断 */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* 等待上一次对RTC寄存器的写操作完成 */
    RTC_WaitForLastTask();
    /* 使能RTC的闹钟中断 */
    RTC_ITConfig(RTC_IT_ALR, ENABLE);
    /* 等待上一次对RTC寄存器的写操作完成 */
    RTC_WaitForLastTask();
}

/**-------------------------------------------------------
  * @函数名 RTC_NVIC_Configuration
  * @功能   RTC实时时钟的中断参数配置函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
static void RTC_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**-------------------------------------------------------
  * @函数名 sys_rtcSetTime
  * @功能   把时间转化为RTC计数值写入RTC寄存器
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void sys_rtcSetTime(uint32_t time)
{
    /* 等待上一次对RTC寄存器的写操作完成 */
    RTC_WaitForLastTask();
    /* 把时间转化为RTC计数值写入RTC寄存器 */
    RTC_SetCounter(time);
    /* 等待上一次对RTC寄存器的写操作完成 */
    RTC_WaitForLastTask();
}

//#define USE_UCOS_II
#ifdef USE_UCOS_II
extern void OSIntEnter(void);
extern void OSIntExit(void);
#else
#define OSIntEnter()
#define OSIntExit()
#endif

/**-------------------------------------------------------
  * @函数名 RTC_IRQHandler
  * @功能   RTC实时时钟的中断处理函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void RTC_IRQHandler(void)
{
    OSIntEnter();
    /*  判断RTC是否发生了秒中断（也有可能是溢出或者闹钟中断) */
    if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        RTC_ClearITPendingBit(RTC_IT_SEC);
        /* 置全局变量为1，通知主程序打印时间 */
        b_rtcTimeDisplayFlag = 1;
        /*RTC已加入日期，无需零点清零RTC寄存器*/
//    /* 等待上一次对RTC寄存器的写操作完成 */
//    RTC_WaitForLastTask();
//    /* 如果时间达到23:59:59则下一刻时间为00:00:00 */
//    if (RTC_GetCounter() == 0x00015180)
//    {
//      RTC_SetCounter(0x0);
//      /* 等待上一次对RTC寄存器的写操作完成 */
//      RTC_WaitForLastTask();
//    }
    }
    /*判断是否发生闹钟中断*/
    else if(RTC_GetITStatus(RTC_IT_ALR) == SET)
    {
        RTC_ClearITPendingBit(RTC_IT_ALR);
        b_rtcAlarmRingFlag = 1;
    }
    OSIntExit();
}
