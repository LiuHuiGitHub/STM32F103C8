#ifndef __DRIVE_H__
#define __DRIVE_H__

#include "typedef.h"

#define ONE_TIME			1680u			//us						//data one is high 1.68ms level and cycle 2.24ms
#define ZERO_TIME			560u			//us						//data zero is high 0.56ms level and cycle 1.12ms
#define LOW_TIME			560u			//us
#define ERROR_TIME			100u			//us

#define START_HIGH_TIME		9000u			//us						//start flag  9ms high level and 4.5ms low level
#define START_LOW_TIME		4500u			//us						//start flag  9ms high level and 4.5ms low level
#define START_REPEAT_TIME	2200u			//us						//repeat send flag 9ms high level and 2.2ms low level

#define START_ERROR_TIME	300u			//us

#define DOWN_OVER_CNT		3u				//repeat send count = DOWN_OVER_CNT , set b_irDownFlag

extern BOOL b_irRevFlag;					//receive success flag
extern BOOL b_irDownFlag;					//repeat send flag
extern UINT8 u8_irRecAdd1;
extern UINT8 u8_irRecAdd2;
extern UINT8 u8_irRecData1;
extern UINT8 u8_irRecData2;

extern void drv_irInit(void);
extern void drv_irHandler(void);		//rising and falling trigger interrupt handle

#endif
