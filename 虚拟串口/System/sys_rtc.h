/*********************************************************************************
 * 文件名  ：sys_rtc.h
 * 描述    ：实时时钟模块
 * 库版本  ：ST3.5.0
 * 编写时间：2013年12月17日
 * 编写人  ：LiuHui
 *
 *
 *使用示例
	if (TimeDisplay == 1){
		time=RTC_GetCounter();
		sprintf(ch,"Time Is:%2d:%2d:%2d",(time/3600)%24,(time%3600)/60,(time%3600)%60);
		drv_lcdShowString(20,20,(uint8_t*)ch,Color[i]);
		TimeDisplay = 0;
	}
**********************************************************************************/
#ifndef __SYS_RTC_H__
#define __SYS_RTC_H__

#define RTC_CLK_HSE128						0			//外部高速晶振128分频
#define RTC_CLK_LSE							1			//外部32.768k晶振
#define RTC_CLK_LSI							0			//内部40k晶振

extern volatile u8 b_rtcTimeDisplayFlag;	//1s时间到达标志
extern volatile u8 b_rtcAlarmRingFlag;//闹钟时间当标志

extern void sys_rtcInit(void);
extern void sys_rtcSetTime(uint32_t time);
extern void sys_rtcSetAlarm(uint32_t AlarmValue);		//设置闹钟时间

#endif
