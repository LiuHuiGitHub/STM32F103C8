#ifndef __DRIVE_E2_H__
#define	__DRIVE_E2_H__

#include "typedef.h"


/* EEPROM Addresses defines */
#define EEPROM_Block0_ADDRESS 0xA0   /* E2 = 0 */
//#define EEPROM_Block1_ADDRESS 0xA2 /* E2 = 0 */
//#define EEPROM_Block2_ADDRESS 0xA4 /* E2 = 0 */
//#define EEPROM_Block3_ADDRESS 0xA6 /* E2 = 0 */

void dr_e2Init(void);
void dr_e2WriteBuffer(UINT8 *pBuffer, UINT8 WriteAddr, UINT16 NumByteToWrite);
void dr_e2WriteByte(UINT8 *pBuffer, UINT8 WriteAddr);
void dr_e2WritePage(UINT8 *pBuffer, UINT8 WriteAddr, UINT8 NumByteToWrite);
void dr_e2ReadBuffer(UINT8 *pBuffer, UINT8 ReadAddr, UINT16 NumByteToRead);
void dr_e2WaitStandby(void);

#endif /* __DRIVE_E2_H__ */
