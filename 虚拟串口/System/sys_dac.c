#include "stm32f10x.h"
#include "sys_dac.h"

void sys_dacInit(void)
{
    DAC_InitTypeDef DAC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    DAC_Cmd(DAC_Channel_1, ENABLE);
    sys_dacOutput(0);
}

/*******************************************************************************
* 函数名称: sys_dacOutput      // 函数名称。
* 说明:          // 函数功能、性能等的描述。
* 输入参数:      // 输入参数说明，包括每个参数的作。
*                // 用、取值说明及参数间关系。
* 输出参数:      // 对输出参数的说明。
* 返回值:        // 函数返回值的说明。
* 其它:          // 其它说明（选填）。
*******************************************************************************/
/*******************************************************************************
* function:	sys_dacOutput
* 说明:
* input:	none
*
* output:	none
* return:	none
* other:	none
*******************************************************************************/
void sys_dacOutput(uint16_t value)
{
    DAC_SetChannel1Data(DAC_Align_12b_R, value);
}

//void DAC_DMA_Out_Config(void)
//{
//	DAC_InitTypeDef DAC_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
//	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
//	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
//	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
//	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
//	DAC_Cmd(DAC_Channel_1, ENABLE);
//	DAC_SetChannel1Data(DAC_Align_12b_R, 0);
//}
//void DAC_Triangle_Config(void)
//{
//	DAC_InitTypeDef DAC_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
//	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
//	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
//	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
//	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
//	DAC_Cmd(DAC_Channel_1, ENABLE);
//	DAC_SetChannel1Data(DAC_Align_12b_R, 0);
//}
