#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "stm32f10x.h"
#include "Typedef.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "math.h"

/*LED*/
//#define LED1					PDout(9)
//#define LED2					PCout(5)
#define LED1					PBout(8)

/*独立按键*/
#define KEY_WKUP			PAin(0)						//高电平有效
#define KEY_S2				PCin(2)
#define KEY_S3				PCin(1)

//#define KEY_Up		PDin(4)
//#define KEY_Down	PDin(1)
//#define KEY_Left	PDin(2)
//#define KEY_Right	PDin(3)
//#define KEY_Sel		PDin(0)

/*函数声明*/
/*初始化函数*/
void SYSTEM_Init(void);
void sys_GPIO_Init(GPIO_TypeDef *GPIOx, \
                   UINT16 GPIO_Pin, \
                   GPIOMode_TypeDef GPIO_Mode, \
                   GPIOSpeed_TypeDef GPIO_Speed);

/*简单延时函数*/
void Delay_us(uint16_t xus);
void Delay_ms(uint16_t xms);

#endif
