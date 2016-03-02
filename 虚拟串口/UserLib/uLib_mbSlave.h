#ifndef __uLib_mb_Slave_H__
#define __uLib_mb_Slave_H__

#include "typedef.h"

void uLib_mbSlaveInit(UINT8 addr);
void uLib_mbReadReqADU(UINT8 *dat, UINT16 len);
void uLib_mbSlaveHandler(void);

#endif
