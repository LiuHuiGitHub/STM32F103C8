#include "stm32f10x.h"
#include "sys_adc.h"

volatile u16 u16_adcConvertedValue[SAMPLE_NUM][CHANNEL_NUM];

static void ADC_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&u16_adcConvertedValue;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = SAMPLE_NUM * CHANNEL_NUM;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

void sys_adcInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    ADC_DMA_Config();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC1和ADC2工作在独立模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;//多通道
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//软件启动转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//转换结果右对齐
    ADC_InitStructure.ADC_NbrOfChannel = CHANNEL_NUM;//通道数目
    ADC_Init(ADC1, &ADC_InitStructure);
//	ADC_TempSensorVrefintCmd(ENABLE);//使能片内温度传感器
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);	//PCLK 8分频
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);//通道，转换次序，转换时间
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 4, ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 5, ADC_SampleTime_239Cycles5);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);//开始校准
    while(ADC_GetCalibrationStatus(ADC1));
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);//使能ADC的软件转换启动功能
}

uint16_t sys_adcGetValue(uint8_t Channel)
{
    uint8_t i;
    uint32_t sum = 0;
    for(i = 0; i < SAMPLE_NUM; i++)
    {
        sum += u16_adcConvertedValue[i][Channel];
    }
    return (sum / SAMPLE_NUM);
}
