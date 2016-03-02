#ifndef __uLib_mb_Slave_Itf_H__
#define __uLib_mb_Slave_Itf_H__

#include "typedef.h"

void uLib_mbSlaveItfInit(void);
void uLib_mbSlaveItfReceiveCtrl(BOOL state);
void uLib_mbSlaveItfTransmitData(UINT8 *dat, UINT16 len);
void uLib_mbSlaveItfHandler(void);

#endif
