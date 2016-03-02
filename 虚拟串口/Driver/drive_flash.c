/*********************************************************************************
 * 文件名  ：drive_flash.c
 * 描述    ：W25X16读写
 * 硬件连接：--------------
 *          | PA5  -  CLK  |
 *          | PA6  -  MISO |
 *          | PA7  -  MOSI |
 *          | PA3  -  CS   |
 *           --------------
 * 库版本  ：ST3.5.0
 * 编写时间：2013年12月6日
 * 编写人  ：LiuHui
**********************************************************************************/
#include "stm32f10x.h"
#include "drive_flash.h"

#define FlashPageByte		256

#define SPI_W25X_CS_Low()    GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define SPI_W25X_CS_High()   GPIO_SetBits(GPIOA, GPIO_Pin_3)


void dr_flashInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;
    /*
    我想使用SPI1并重映射引脚使用时注意的事项如下:
    1.使能PB0时钟 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    2.使能AFIO时钟RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    3.使能SPI1时钟RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 , ENABLE);
    4.禁用JTAG时钟复用 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
    5.SPI1引脚重映射 GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
    */
    /* Enable SPI1 GPIOA clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 , ENABLE);
//  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
//  GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
    /* Configure SPI1 pins: SCKand MOSI */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure PC3 as Output push-pull, used as Flash Chip select */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);
    dr_flashReadDeviceID();
}

/********************************************************
 * 函数名：dr_flashSendByte
 * 描述  ：传送1Byte数据
 * 输入  ：1Byte数据
 * 输出  ：1Byte数据
 ********************************************************/
u8 dr_flashSendByte(u8 byte)
{
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, byte);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI1);
}

/********************************************************
 * 函数名：dr_flashCmd
 * 描述  ：W25X写控制
 * 输入  ：ENABLE 使能   DISABLE 禁能
 * 输出  ：无
 ********************************************************/
void dr_flashCmd(FunctionalState NowState)
{
    SPI_W25X_CS_Low();
    if(NowState)
    {
        dr_flashSendByte(W25X_Write_ENABLE);
    }
    else
    {
        dr_flashSendByte(W25X_Write_ENABLE);
    }
    SPI_W25X_CS_High();
}

/********************************************************
 * 函数名：dr_flashReadDeviceID
 * 描述  ：读W25X_ID 或 释放掉电
 * 输入  ：无
 * 输出  ：W25X16为14H   W25X32为15H   W25X64为16H
 ********************************************************/
u8 dr_flashReadDeviceID(void)
{
    u8 ID;
    SPI_W25X_CS_Low();
    dr_flashSendByte(W25X_Device_ID);
    dr_flashSendByte(Dummy_Byte);
    dr_flashSendByte(Dummy_Byte);
    dr_flashSendByte(Dummy_Byte);
    ID = dr_flashSendByte(Dummy_Byte);
    SPI_W25X_CS_High();
    return ID;
}

/********************************************************
 * 函数名：dr_flashReadJedecID
 * 描述  ：读制造/器件号
 * 输入  ：无
 * 输出  ：W25X16为3015H   W25X32为3016H   W25X64为3017H
 ********************************************************/
u32 dr_flashReadJedecID(void)
{
    u32 ID;
    SPI_W25X_CS_Low();
    dr_flashSendByte(W25X_JEDEC_ID);
    ID = dr_flashSendByte(Dummy_Byte);
    ID <<= 8;
    ID |= dr_flashSendByte(Dummy_Byte);
    ID <<= 8;
    ID |= dr_flashSendByte(Dummy_Byte);
    SPI_W25X_CS_High();
    return ID;
}

/********************************************************
 * 函数名：dr_flashReadStatus
 * 描述  ：读W25X寄存器状态
 * 输入  ：无
 * 输出  ：
 ********************************************************/
u8 dr_flashReadStatus(void)
{
    u8 tmp;
    SPI_W25X_CS_Low();
    dr_flashSendByte(W25X_Read_Status_Register);
    tmp = dr_flashSendByte(Dummy_Byte);
    SPI_W25X_CS_High();
    return tmp;
}

/********************************************************
 * 函数名：dr_flashWriteStatus
 * 描述  ：写W25X寄存器状态
 * 输入  ：
 * 输出  ：无
 ********************************************************/
void dr_flashWriteStatus(u8 NowStatus)
{
    dr_flashCmd(ENABLE);
    SPI_W25X_CS_Low();
    dr_flashSendByte(W25X_Write_Status_Register);
    dr_flashSendByte(NowStatus);
    SPI_W25X_CS_High();
}

/********************************************************
 * 函数名：dr_flashErase
 * 描述  ：清除W25X数据
 * 输入  ：EraseAddr要清除的首地址
 * 输出  ：Rangge 清除范围 (Sector  4K   Block  64K   Chip  全部)
 ********************************************************/
void dr_flashErase(u32 EraseAddr, EraseRange Rangge)
{
    dr_flashCmd(ENABLE);
    SPI_W25X_CS_Low();
    dr_flashSendByte(Rangge);
    if(Rangge != Chip)
    {
        dr_flashSendByte((EraseAddr & 0xff0000) >> 16);
        dr_flashSendByte((EraseAddr & 0xff00) >> 8);
        dr_flashSendByte(EraseAddr & 0xff);
    }
    SPI_W25X_CS_High();
    while(dr_flashReadStatus() & 0x01);
}

/********************************************************
 * 函数名：dr_flashWriteData
 * 描述  ：向W25X芯片写数据
 * 输入  ：Data      要写的数据
 *         WriteAddr 要写的地址
 *         WriteByte 写入的字节数
 * 输出  ：无
 * 说明  ：每次写入数据前需清除原有数据。
 *         W25X每页256字节,WriteByte小于等于256
 ********************************************************/
void dr_flashWritePage(u8 *Data, u32 WriteAddr, u16 WriteByte)
{
    dr_flashCmd(ENABLE);
    SPI_W25X_CS_Low();
    dr_flashSendByte(W25X_Page_Program);
    dr_flashSendByte((WriteAddr & 0xff0000) >> 16);
    dr_flashSendByte((WriteAddr & 0xff00) >> 8);
    dr_flashSendByte(WriteAddr & 0xff);
    if(WriteByte >= FlashPageByte)
    {
        WriteByte = 256;
    }
    while(WriteByte--)
    {
        dr_flashSendByte(*Data);
        Data++;
    }
    SPI_W25X_CS_High();
    while(dr_flashReadStatus() & 0x01);
}

/********************************************************
 * 函数名：dr_flashReadData
 * 描述  ：从W25X芯片读数据
 * 输入  ：Data      读出的数据存放位置
 *         ReadAddr  读取的地址
 *         ReadByte  读取的字节数
 * 输出  ：无
 * 说明  ：W25X可以一次从头读到末尾
 ********************************************************/
void dr_flashReadData(u8 *Data, u32 ReadAddr, u32 ReadByte)
{
    SPI_W25X_CS_Low();
    dr_flashSendByte(W25X_Read_Data);
    dr_flashSendByte((ReadAddr & 0xff0000) >> 16);
    dr_flashSendByte((ReadAddr & 0xff00) >> 8);
    dr_flashSendByte(ReadAddr & 0xff);
    while(ReadByte--)
    {
        *Data = dr_flashSendByte(Dummy_Byte);
        Data++;
    }
    SPI_W25X_CS_High();
    while(dr_flashReadStatus()&Busy_Flag);
}
/********************************************************
 * 函数名：SPI_Write_Buffer
 * 描述  ：向W25X芯片写数据
 * 输入  ：Data      要写的数据
 *         WriteAddr 要写的地址
 *         WriteByte 写入的字节数
 * 输出  ：无
 * 说明  ：每次写入数据前需清除原有数据。可以写到底
 ********************************************************/
void dr_flashWriteData(u8 *Data, u32 WriteAddr, u32 WriteByte)
{
    u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
    Addr = WriteAddr % FlashPageByte;
    count = FlashPageByte - Addr;
    NumOfPage = WriteByte / FlashPageByte;
    NumOfSingle = WriteByte % FlashPageByte;
    if(Addr == 0)
    {
        if(NumOfPage == 0)
        {
            dr_flashWritePage(Data, WriteAddr, WriteByte);
        }
        else
        {
            while(NumOfPage--)
            {
                dr_flashWritePage(Data, WriteAddr, FlashPageByte);
                WriteAddr += FlashPageByte;
                Data += FlashPageByte;
            }
            dr_flashWritePage(Data, WriteAddr, NumOfSingle);
        }
    }
    else
    {
        if(NumOfPage == 0)
        {
            if(NumOfSingle > count)
            {
                temp = NumOfSingle - count;
                dr_flashWritePage(Data, WriteAddr, count);
                WriteAddr += count;
                Data += count;
                dr_flashWritePage(Data, WriteAddr, temp);
            }
            else
            {
                dr_flashWritePage(Data, WriteAddr, WriteByte);
            }
        }
        else
        {
            WriteByte -= count;
            NumOfPage = WriteByte / FlashPageByte;
            NumOfSingle = WriteByte % FlashPageByte;
            dr_flashWritePage(Data, WriteAddr, count);
            WriteAddr += count;
            Data += count;
            while(NumOfPage--)
            {
                dr_flashWritePage(Data, WriteAddr, FlashPageByte);
                WriteAddr += FlashPageByte;
                Data += FlashPageByte;
            }
            if(NumOfSingle != 0)
            {
                dr_flashWritePage(Data, WriteAddr, NumOfSingle);
            }
        }
    }
}

