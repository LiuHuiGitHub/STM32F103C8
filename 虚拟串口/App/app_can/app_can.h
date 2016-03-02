#ifndef __APP_CAN_H__
#define __APP_CAN_H__

#include "CanMsg.h"

void app_canInit(void);
void app_canHandler1ms(void);
void app_canLight(Light_ENUM light, BOOL sw);
void app_canCircle(Circle_ENUM Circle, UINT8 angle);

#endif
