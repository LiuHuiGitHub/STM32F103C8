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
 *
*/
/*使用示例：
	dr_flashInit();
	dr_flashErase(0, Block);
	dr_flashWriteData(chi,0,500);
	sprintf((char*)ch,"%x",dr_flashReadJedecID());
	drv_lcdShowString(20,20,(uint8_t*)ch,Color[0]);
	dr_flashReadData((uint8_t*)ch,0,500);
	drv_lcdShowString(0,0,(uint8_t*)ch,Color[0]);
*/
/**********************************************************************************/
#ifndef __DRIVE_FLASH_H__
#define __DRIVE_FLASH_H__

typedef enum {Sector = 0x20, Block = 0xd8, Chip = 0xc7} EraseRange;

#define Dummy_Byte                  0xA5
#define Busy_Flag                   0x01

#define W25X_Write_ENABLE						0x06		//写势能
#define W25X_Write_DISABLE					0x04		//写禁能
#define W25X_Read_Status_Register		0x05    //读状态寄存器
#define W25X_Write_Status_Register	0x01    //写状态寄存器
#define W25X_Read_Data							0x03    //读数据
#define W25X_Fast_Read              0x0b    //快读
#define W25X_Fasr_Read_Dual_Output	0x3b    //快读双输出
#define W25X_Page_Program						0x02		//页编程
#define W25X_Block_Erase						0xd8		//块擦除  64K
#define W25X_Sector_Rease						0x20		//扇区擦除 4K
#define W25X_Chip_Erase							0xc7		//芯片擦除
#define W25X_Power_Down							0xb9		//掉电
#define W25X_Device_ID							0xAB		//释放掉电/器件ID
#define W25X_JEDEC_ID								0x9f		//器件JEDEC_ID

void dr_flashInit(void);
/*传送1Byte数据*/
u8 dr_flashSendByte(u8 byte);
/*W25X写控制   ENABLE 使能   DISABLE 禁能*/
void dr_flashCmd(FunctionalState NowState);
/*读W25X_Device_ID*/
u8 dr_flashReadDeviceID(void);
/*读W25x_JEDEC_ID*/
u32 dr_flashReadJedecID(void);
/*读W25X寄存器状态*/
u8 dr_flashReadStatus(void);
/*写W25X寄存器状态*/
void dr_flashWriteStatus(u8 NowStatus);
/*清除W25X数据
EraseAddr清除地址
Rangge 清除范围(Sector  4K   Block  64K   Chip  全部)*/
void dr_flashErase(u32 EraseAddr, EraseRange Rangge);
/*向W25X芯片写数据*/
void dr_flashWritePage(u8 *Data, u32 WriteAddr, u16 WriteByte);
/*从W25X芯片读数据*/
void dr_flashReadData(u8 *Data, u32 ReadAddr, u32 ReadByte);
/*向W25X芯片写数据*/
void dr_flashWriteData(u8 *pBuffer, u32 WriteAddr, u32 NumByteToWrite);
#endif
