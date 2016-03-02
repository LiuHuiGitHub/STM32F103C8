/******************** (C) COPYRIGHT 2013 www.armjishu.com  ********************
 * 文件名  ：drive_touch.h
 * 描述    ：实现STM32F107VC神舟IV号开发板的触摸屏功能函数
 * 实验平台：STM32神舟开发板
 * 标准库  ：STM32F10x_StdPeriph_Driver V3.5.0
 * 作者    ：www.armjishu.com
*******************************************************************************/

#ifndef __DRIVE_TOUCH_H__
#define __DRIVE_TOUCH_H__

/* Includes ------------------------------------------------------------------*/

/* 按键状态	*/
#define Pen_Down 0x01
#define Pen_Up   0x00
/* 笔杆结构体 */
typedef struct
{
    u16 X0;//原始坐标
    u16 Y0;
    u16 X; //最终/暂存坐标
    u16 Y;
    u8  Key_Sta;//笔的状态
    //触摸屏校准参数
    float xfac;
    float yfac;
    short xoff;
    short yoff;
} Pen_Holder;

extern Pen_Holder Pen_Point;


void drv_touchInit(void);
void drv_touchAdjust(void);
void drv_touchReadCoord(void);
void drv_touchHandler(void);

#endif

/*=====================================================================*/
