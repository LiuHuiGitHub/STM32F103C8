#ifndef __SYS_PWM_H__
#define __SYS_PWM_H__

#include "typedef.h"

#define ENABLE_OC1
//#define ENABLE_OC2
//#define ENABLE_OC3
#define ENABLE_OC4

void sys_pwmInit(UINT32 freq);
void sys_pwmSetRatio(UINT8 Channal, UINT8 ratio);

#endif
