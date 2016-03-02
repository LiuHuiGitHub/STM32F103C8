#ifndef __SYS_USART_H__
#define __SYS_USART_H__

#include "stm32f10x.h"

#define USART1_REMAP								0   //0不映射
#define USART2_REMAP								1
#define USART3_PARTIALREMAP					        0   //部分重映射
#define USART3_FULLREMAP						    0   //完全重映射

void sys_usartInit(USART_TypeDef *USART_x, u32 Boud);
void sys_usartSendData(uint8_t *Data, uint32_t SendSize);
void sys_usartTransmit(USART_TypeDef *Usartx, uint8_t *ptr, uint16_t size);

#endif
