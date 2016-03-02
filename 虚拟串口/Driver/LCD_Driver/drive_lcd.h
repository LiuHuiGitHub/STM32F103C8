#ifndef  __DRIVE_LCD_H__
#define  __DRIVE_LCD_H__
#include "typedef.h"

//TFTLCD部分外要调用的函数
extern UINT16  POINT_COLOR;//默认红色
extern UINT16  BACK_COLOR; //背景颜色.默认为白色

//定义LCD的尺寸
#define LCD_W 240
#define LCD_H 320

/***********************************************************************************
设置颜色宏定义
***********************************************************************************/
#define   BLACK        0x0000                    /* 黑色： 0, 0, 0               */
#define   NAVY         0x000F                    /* 深蓝色： 0, 0, 128           */
#define   DGREEN       0x03E0                    /* 深绿色： 0, 128, 0           */
#define   DCYAN        0x03EF                    /* 深青色： 0, 128, 128         */
#define   MAROON       0x7800                    /* 深红色：128, 0, 0            */
#define   PURPLE       0x780F                    /* 紫色： 128, 0, 128           */
#define   OLIVE        0x7BE0                    /* 橄榄绿：128, 128, 0          */
#define   LGRAY        0xC618                    /* 灰白色：192, 192, 192        */
#define   DGRAY        0x7BEF                    /* 深灰色：128, 128, 128        */
#define   BLUE         0x001F                    /* 蓝色： 0, 0, 255             */
#define   GREEN        0x07E0                 	 /* 绿色： 0, 255, 0             */
#define   CYAN         0x07FF                    /* 青色： 0, 255, 255           */
#define   RED          0xF800                    /* 红色： 255, 0, 0             */
#define   MAGENTA      0xF81F                    /* 品红： 255, 0, 255           */
#define   YELLOW       0xFFE0                    /* 黄色： 255, 255, 0           */
#define   WHITE        0xFFFF                    /* 白色： 255, 255, 255         */
#define   IDMCOLOR(color) (((color & 0x001F) << 11) | ((color & 0xF800) >> 11) | (color & 0x07E0))

//#define   BACK_COLOR    BLACK                    /* 定义清屏色  				 */
//#define   POINT_COLOR   WHITE                   /* 定义清屏色  				 */


/* 函数声明 */
void drv_lcdDrawPoint(UINT16 x, UINT16 y);
void drv_lcdDrawBigPoint(UINT16 x, UINT16 y);
void drv_lcdSetCursor(UINT8 Xpos, UINT16 Ypos);
void drv_lcdInit(void);
void drv_lcdSetWindow(UINT8 x, UINT16 y, UINT8 x_end, UINT16 y_end);
void drv_lcdShowString(UINT8 x, UINT16 y, UINT8 *p);
void LCD_ShowNum(UINT8 x, UINT16 y, UINT32 num, UINT8 len, UINT8 size);
void LCD_ShowChar(UINT8 x, UINT16 y, UINT8 chars, UINT8 size, UINT8 mode);
void drv_lcdShowPicture(UINT8 Xpos, UINT16 Ypos, UINT8 Height, UINT16 Width, UINT8 *bitmap);
void drv_lcdClear(UINT16 Color);
UINT16 drv_lcdWriteOneASCII(UINT8 *pucMsk,
                            UINT16 x0,
                            UINT16 y0,
                            UINT16 color);
UINT16 drv_lcdWriteOneHzChar(UINT8 *pucMsk,
                             UINT16 x0,
                             UINT16 y0,
                             UINT16 color);
void drv_lcdWriteString(UINT16 x0, UINT16 y0, UINT8 *pcStr, UINT16 color);
void drv_lcdDrawLine(UINT16 x1, UINT16 y1, UINT16 x2, UINT16 y2);
void drv_lcdDrawCircle(UINT8 x0, UINT16 y0, UINT8 r);

/***********************************************************************************
9320/9325 LCD
***********************************************************************************/
#define R0             0x00
#define R1             0x01
#define R2             0x02
#define R3             0x03
#define R4             0x04
#define R5             0x05
#define R6             0x06
#define R7             0x07
#define R8             0x08
#define R9             0x09
#define R10            0x0A
#define R12            0x0C
#define R13            0x0D
#define R14            0x0E
#define R15            0x0F
#define R16            0x10
#define R17            0x11
#define R18            0x12
#define R19            0x13
#define R20            0x14
#define R21            0x15
#define R22            0x16
#define R23            0x17
#define R24            0x18
#define R25            0x19
#define R26            0x1A
#define R27            0x1B
#define R28            0x1C
#define R29            0x1D
#define R30            0x1E
#define R31            0x1F
#define R32            0x20
#define R33            0x21
#define R34            0x22
#define R36            0x24
#define R37            0x25
#define R40            0x28
#define R41            0x29
#define R43            0x2B
#define R45            0x2D
#define R48            0x30
#define R49            0x31
#define R50            0x32
#define R51            0x33
#define R52            0x34
#define R53            0x35
#define R54            0x36
#define R55            0x37
#define R56            0x38
#define R57            0x39
#define R59            0x3B
#define R60            0x3C
#define R61            0x3D
#define R62            0x3E
#define R63            0x3F
#define R64            0x40
#define R65            0x41
#define R66            0x42
#define R67            0x43
#define R68            0x44
#define R69            0x45
#define R70            0x46
#define R71            0x47
#define R72            0x48
#define R73            0x49
#define R74            0x4A
#define R75            0x4B
#define R76            0x4C
#define R77            0x4D
#define R78            0x4E
#define R79            0x4F
#define R80            0x50
#define R81            0x51
#define R82            0x52
#define R83            0x53
#define R96            0x60
#define R97            0x61
#define R106           0x6A
#define R118           0x76
#define R128           0x80
#define R129           0x81
#define R130           0x82
#define R131           0x83
#define R132           0x84
#define R133           0x85
#define R134           0x86
#define R135           0x87
#define R136           0x88
#define R137           0x89
#define R139           0x8B
#define R140           0x8C
#define R141           0x8D
#define R143           0x8F
#define R144           0x90
#define R145           0x91
#define R146           0x92
#define R147           0x93
#define R148           0x94
#define R149           0x95
#define R150           0x96
#define R151           0x97
#define R152           0x98
#define R153           0x99
#define R154           0x9A
#define R157           0x9D
#define R192           0xC0
#define R193           0xC1
#define R229           0xE5

#endif
/****************************** End File ********************************/
