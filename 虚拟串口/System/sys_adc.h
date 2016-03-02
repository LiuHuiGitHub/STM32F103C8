#ifndef __DRIVE_ADC_H__
#define __DRIVE_ADC_H__

#define CHANNEL_NUM		2
#define SAMPLE_NUM		50

extern volatile u16 u16_adcConvertedValue[SAMPLE_NUM][CHANNEL_NUM];

void sys_adcInit(void);
uint16_t sys_adcGetValue(uint8_t Channel);

#endif
