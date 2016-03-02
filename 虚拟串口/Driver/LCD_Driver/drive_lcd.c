/**************************************************************
** 	火牛开发板
**	LCD驱动代码
**  版本：V1.0
**	论坛：www.openmcu.com
**	淘宝：http://shop36995246.taobao.com/
**  技术支持群：121939788
***************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "Typedef.h"
#include "drive_lcd.h"
#include "Font_Lib.h"	 //字库文件库
#include "hz16.c"

/* LCD 控制引脚配置*/
#define LCD_CS  PBout(0)
#define LCD_RS	PDout(9)
#define LCD_WR  PBout(1)
#define LCD_RD	PDout(10)

UINT16 POINT_COLOR = BLUE, BACK_COLOR = WHITE; /* 分别设置点的颜色和底色	*/
UINT16 DeviceCode;

extern void Delay_us(uint16_t xus);
extern void Delay_ms(uint16_t xms);
static void LCD_IO_Init(void);
static void Write_Cmd(UINT16 LCD_Reg);
static void Write_Dat(UINT16 LCD_Dat);
static UINT16 LCD_ReadDat(void);
static UINT16 LCD_ReadReg(UINT16 LCD_Reg);
static void drv_lcdWriteRAMPrepare(void);
static void LCD_WriteReg(UINT16 LCD_Reg, UINT16 LCD_Dat);
static void LCD_WriteReg(UINT16 LCD_Reg, UINT16 LCD_Dat);

/*****************************************************************************
** 函数名称: LCD_Write_Reg
** 功能描述: 写指令及数据
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void LCD_WriteReg(UINT16 LCD_Reg, UINT16 LCD_Dat)
{
    Write_Cmd(LCD_Reg);
    Write_Dat(LCD_Dat);
}
/*****************************************************************************
** 函数名称: Write_Cmd
** 功能描述: 写指令
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void Write_Cmd(UINT16 LCD_Reg)
{
    LCD_CS = 0;
    LCD_RS = 0;
    GPIOE->ODR = LCD_Reg;
    LCD_WR = 0;
    LCD_WR = 1;
    LCD_CS = 1;
}
/*****************************************************************************
** 函数名称: Write_Dat
** 功能描述: 写数据
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void Write_Dat(UINT16 LCD_Dat)
{
    LCD_CS = 0;
    LCD_RS = 1;
    GPIOE->ODR = LCD_Dat;
    LCD_WR = 0;
    LCD_WR = 1;
    LCD_CS = 1;
}
/*****************************************************************************
** 函数名称: LCD_ReadReg
** 功能描述: 读指令
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
UINT16 LCD_ReadReg(UINT16 LCD_Reg)
{
    Write_Cmd(LCD_Reg);  //写入要读的寄存器号
    return LCD_ReadDat();
}
/*****************************************************************************
** 函数名称: LCD_ReadDat
** 功能描述: 读数据
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
UINT16 LCD_ReadDat(void)
{
    UINT16 temp;
    GPIOE->CRH = (GPIOE->CRH & 0x00000000) | 0x44444444;  //将端口高8位配置成输入
    GPIOE->CRL = (GPIOE->CRL & 0x00000000) | 0x44444444;  //将端口低8位配置成输入
    LCD_CS = 0;
    LCD_RS = 1;
    LCD_RD = 0;
    temp = GPIOE->IDR; 	//读取数据(读寄存器时,并不需要读2次)
    //temp = GPIOE->IDR;
    LCD_RD = 1;
    LCD_CS = 1;
    GPIOE->CRH = (GPIOE->CRH & 0x00000000) | 0x33333333;  //释放端口高8位为输出
    GPIOE->CRL = (GPIOE->CRL & 0x00000000) | 0x33333333;  //释放端口低8位为输出
    return temp;
}
/*****************************************************************************
** 函数名称: LCD_IO_Init
** 功能描述: LCD_IO口配置
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void LCD_IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
    /* 配置控制IO 连接到PD12.PD13.PD14.PD15 *********************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // 推挽输出方式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 输出IO口最大最速为50MHZ
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // 推挽输出方式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 输出IO口最大最速为50MHZ
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    /* 配置数据IO 连接到GPIOE *********************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // 推挽输出方式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 输出IO口最大最速为50MHZ
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}
/*****************************************************************************
** 函数名称: drv_lcdInit
** 功能描述: LCD初始化
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void drv_lcdInit(void)
{
    LCD_IO_Init();
    LCD_WriteReg(0x0000, 0x0001);
    Delay_ms(50); // LCD_Delay 50 ms
    DeviceCode = LCD_ReadReg(0x0000);
    //printf("ID:%d\n",DeviceCode);
    if(DeviceCode == 0x9325 || DeviceCode == 0x9328) //ILI9325
    {
        LCD_WriteReg(0x00e7, 0x0010);
        LCD_WriteReg(0x0000, 0x0001); //开启内部时钟
        LCD_WriteReg(0x0001, 0x0100);
        LCD_WriteReg(0x0002, 0x0700); //电源开启
        //LCD_WriteReg(0x0003,(1<<3)|(1<<4) ); 	//65K  RGB
        //DRIVE TABLE(寄存器 03H)
        //BIT3=AM BIT4:5=ID0:1
        //AM ID0 ID1   FUNCATION
        // 0  0   0	   R->L D->U
        // 1  0   0	   D->U	R->L
        // 0  1   0	   L->R D->U
        // 1  1   0    D->U	L->R
        // 0  0   1	   R->L U->D
        // 1  0   1    U->D	R->L
        // 0  1   1    L->R U->D 正常就用这个.水平跟新数据
        // 1  1   1	   U->D	L->R  垂直跟新数据
        LCD_WriteReg(0x0003, (1 << 12) | (3 << 4) | (0 << 3)); //65K
        LCD_WriteReg(0x0004, 0x0000);
        LCD_WriteReg(0x0008, 0x0207);
        LCD_WriteReg(0x0009, 0x0000);
        LCD_WriteReg(0x000a, 0x0000); //display setting
        LCD_WriteReg(0x000c, 0x0001); //display setting
        LCD_WriteReg(0x000d, 0x0000); //0f3c
        LCD_WriteReg(0x000f, 0x0000);
        //电源配置
        LCD_WriteReg(0x0010, 0x0000);
        LCD_WriteReg(0x0011, 0x0007);
        LCD_WriteReg(0x0012, 0x0000);
        LCD_WriteReg(0x0013, 0x0000);
        Delay_ms(1);
        LCD_WriteReg(0x0010, 0x1590);
        LCD_WriteReg(0x0011, 0x0227);
        Delay_ms(1);
        LCD_WriteReg(0x0012, 0x009c);
        Delay_ms(1);
        LCD_WriteReg(0x0013, 0x1900);
        LCD_WriteReg(0x0029, 0x0023);
        LCD_WriteReg(0x002b, 0x000e);
        Delay_ms(1);
        LCD_WriteReg(0x0020, 0x0000);
        LCD_WriteReg(0x0021, 0x013f);
        Delay_ms(1);
        //伽马校正
        LCD_WriteReg(0x0030, 0x0007);
        LCD_WriteReg(0x0031, 0x0707);
        LCD_WriteReg(0x0032, 0x0006);
        LCD_WriteReg(0x0035, 0x0704);
        LCD_WriteReg(0x0036, 0x1f04);
        LCD_WriteReg(0x0037, 0x0004);
        LCD_WriteReg(0x0038, 0x0000);
        LCD_WriteReg(0x0039, 0x0706);
        LCD_WriteReg(0x003c, 0x0701);
        LCD_WriteReg(0x003d, 0x000f);
        Delay_ms(1);
        LCD_WriteReg(0x0050, 0x0000); //水平GRAM起始位置
        LCD_WriteReg(0x0051, 0x00ef); //水平GRAM终止位置
        LCD_WriteReg(0x0052, 0x0000); //垂直GRAM起始位置
        LCD_WriteReg(0x0053, 0x013f); //垂直GRAM终止位置
        LCD_WriteReg(0x0060, 0xa700);
        LCD_WriteReg(0x0061, 0x0001);
        LCD_WriteReg(0x006a, 0x0000);
        LCD_WriteReg(0x0080, 0x0000);
        LCD_WriteReg(0x0081, 0x0000);
        LCD_WriteReg(0x0082, 0x0000);
        LCD_WriteReg(0x0083, 0x0000);
        LCD_WriteReg(0x0084, 0x0000);
        LCD_WriteReg(0x0085, 0x0000);
        LCD_WriteReg(0x0090, 0x0010);
        LCD_WriteReg(0x0092, 0x0000);
        LCD_WriteReg(0x0093, 0x0003);
        LCD_WriteReg(0x0095, 0x0110);
        LCD_WriteReg(0x0097, 0x0000);
        LCD_WriteReg(0x0098, 0x0000);
        //开启显示设置
        LCD_WriteReg(0x0007, 0x0133);
        LCD_WriteReg(0x0020, 0x0000);
        LCD_WriteReg(0x0021, 0x013f);
    }
    else if(DeviceCode == 0x9320 || DeviceCode == 0x9300)
    {
        LCD_WriteReg(0x00, 0x0000);
        LCD_WriteReg(0x01, 0x0100);	//Driver Output Contral.
        LCD_WriteReg(0x02, 0x0700);	//LCD Driver Waveform Contral.
        LCD_WriteReg(0x03, 0x1030); //Entry Mode Set.
        //LCD_WriteReg(0x03,0x1018);	//Entry Mode Set.
        LCD_WriteReg(0x04, 0x0000);	//Scalling Contral.
        LCD_WriteReg(0x08, 0x0202);	//Display Contral 2.(0x0207)
        LCD_WriteReg(0x09, 0x0000);	//Display Contral 3.(0x0000)
        LCD_WriteReg(0x0a, 0x0000);	//Frame Cycle Contal.(0x0000)
        LCD_WriteReg(0x0c, (1 << 0));	//Extern Display Interface Contral 1.(0x0000)
        LCD_WriteReg(0x0d, 0x0000);	//Frame Maker Position.
        LCD_WriteReg(0x0f, 0x0000);	//Extern Display Interface Contral 2.
        Delay_ms(1);
        LCD_WriteReg(0x07, 0x0101);	//Display Contral.
        Delay_ms(1);
        LCD_WriteReg(0x10, (1 << 12) | (0 << 8) | (1 << 7) | (1 << 6) | (0 << 4));	//Power Control 1.(0x16b0)
        LCD_WriteReg(0x11, 0x0007);								//Power Control 2.(0x0001)
        LCD_WriteReg(0x12, (1 << 8) | (1 << 4) | (0 << 0));				//Power Control 3.(0x0138)
        LCD_WriteReg(0x13, 0x0b00);								//Power Control 4.
        LCD_WriteReg(0x29, 0x0000);								//Power Control 7.
        LCD_WriteReg(0x2b, (1 << 14) | (1 << 4));
        LCD_WriteReg(0x50, 0);	//Set X Star
        //水平GRAM终止位置Set X End.
        LCD_WriteReg(0x51, 239);	//Set Y Star
        LCD_WriteReg(0x52, 0);	//Set Y End.t.
        LCD_WriteReg(0x53, 319);	//
        LCD_WriteReg(0x60, 0x2700);	//Driver Output Control.
        LCD_WriteReg(0x61, 0x0001);	//Driver Output Control.
        LCD_WriteReg(0x6a, 0x0000);	//Vertical Srcoll Control.
        LCD_WriteReg(0x80, 0x0000);	//Display Position? Partial Display 1.
        LCD_WriteReg(0x81, 0x0000);	//RAM Address Start? Partial Display 1.
        LCD_WriteReg(0x82, 0x0000);	//RAM Address End-Partial Display 1.
        LCD_WriteReg(0x83, 0x0000);	//Displsy Position? Partial Display 2.
        LCD_WriteReg(0x84, 0x0000);	//RAM Address Start? Partial Display 2.
        LCD_WriteReg(0x85, 0x0000);	//RAM Address End? Partial Display 2.
        LCD_WriteReg(0x90, (0 << 7) | (16 << 0));	//Frame Cycle Contral.(0x0013)
        LCD_WriteReg(0x92, 0x0000);	//Panel Interface Contral 2.(0x0000)
        LCD_WriteReg(0x93, 0x0001);	//Panel Interface Contral 3.
        LCD_WriteReg(0x95, 0x0110);	//Frame Cycle Contral.(0x0110)
        LCD_WriteReg(0x97, (0 << 8));	//
        LCD_WriteReg(0x98, 0x0000);	//Frame Cycle Contral.
        LCD_WriteReg(0x07, 0x0173);	//(0x0173)
        Delay_ms(1);
    }
    else if(DeviceCode == 0x8999)	 //对应的驱动IC为SSD1289
    {
        //************* Start Initial Sequence **********//
        LCD_WriteReg(0x00, 0x0001); // Start internal OSC.
        LCD_WriteReg(0x01, 0x3B3F); // Driver output control, RL=0;REV=1;GD=1;BGR=0;SM=0;TB=1
        LCD_WriteReg(0x02, 0x0600); // set 1 line inversion
        //************* Power control setup ************/
        LCD_WriteReg(0x0C, 0x0007); // Adjust VCIX2 output voltage
        LCD_WriteReg(0x0D, 0x0006); // Set amplitude magnification of VLCD63
        LCD_WriteReg(0x0E, 0x3200); // Set alternating amplitude of VCOM
        LCD_WriteReg(0x1E, 0x00BB); // Set VcomH voltage
        LCD_WriteReg(0x03, 0x6A64); // Step-up factor/cycle setting
        //************ RAM position control **********/
        LCD_WriteReg(0x0F, 0x0000); // Gate scan position start at G0.
        LCD_WriteReg(0x44, 0xEF00); // Horizontal RAM address position
        LCD_WriteReg(0x45, 0x0000); // Vertical RAM address start position
        LCD_WriteReg(0x46, 0x013F); // Vertical RAM address end position
        // ----------- Adjust the Gamma Curve ----------//
        LCD_WriteReg(0x30, 0x0000);
        LCD_WriteReg(0x31, 0x0706);
        LCD_WriteReg(0x32, 0x0206);
        LCD_WriteReg(0x33, 0x0300);
        LCD_WriteReg(0x34, 0x0002);
        LCD_WriteReg(0x35, 0x0000);
        LCD_WriteReg(0x36, 0x0707);
        LCD_WriteReg(0x37, 0x0200);
        LCD_WriteReg(0x3A, 0x0908);
        LCD_WriteReg(0x3B, 0x0F0D);
        //************* Special command **************/
        LCD_WriteReg(0x28, 0x0006); // Enable test command
        LCD_WriteReg(0x2F, 0x12EB); // RAM speed tuning
        LCD_WriteReg(0x26, 0x7000); // Internal Bandgap strength
        LCD_WriteReg(0x20, 0xB0E3); // Internal Vcom strength
        LCD_WriteReg(0x27, 0x0044); // Internal Vcomh/VcomL timing
        LCD_WriteReg(0x2E, 0x7E45); // VCOM charge sharing time
        //************* Turn On display ******************/
        LCD_WriteReg(0x10, 0x0000); // Sleep mode off.
        Delay_ms(30); // Wait 30mS
        LCD_WriteReg(0x11, 0x6870); // Entry mode setup. 262K type B, take care on the data bus with 16it only
        LCD_WriteReg(0x07, 0x0033); // Display ON	*/
    }
    drv_lcdClear(BACK_COLOR);
}
/*****************************************************************************
** 函数名称: drv_lcdDrawPoint
** 功能描述: 写一个点
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void drv_lcdDrawPoint(UINT16 x, UINT16 y)
{
    drv_lcdSetCursor(x, y); //设置光标位置
    Write_Cmd(R34);//开始写入GRAM
    Write_Dat(POINT_COLOR);
}
/*****************************************************************************
** 函数名称: drv_lcdWriteRAMPrepare
** 功能描述: 些准备
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void drv_lcdWriteRAMPrepare(void)
{
    Write_Cmd(R34);
}
/*****************************************************************************
** 函数名称: drv_lcdSetCursor
** 功能描述: 设置光标函数
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void drv_lcdSetCursor(UINT8 Xpos, UINT16 Ypos)
{
    if(DeviceCode == 0x8999 || DeviceCode == 0x9919)
    {
        LCD_WriteReg(0x004E, Xpos);
        LCD_WriteReg(0X004F, Ypos);
    }
    else
    {
        LCD_WriteReg(0x0020, Xpos);
        LCD_WriteReg(0X0021, Ypos);
    }
}
/*****************************************************************************
** 函数名称: drv_lcdSetWindow
** 功能描述: 设置窗口函数 (将Width和Height调换一下，前提是)
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void drv_lcdSetWindow(UINT8 x, UINT16 y, UINT8 x_end, UINT16 y_end)
{
    if(DeviceCode == 0x8999)
    {
        LCD_WriteReg(0x44, x | ((x_end + x) << 8));
        LCD_WriteReg(0x45, y);
        LCD_WriteReg(0x46, y + y_end);
    }
    else
    {
        LCD_WriteReg(0x50, x);                      /* Horizontal GRAM Start Address      */
        LCD_WriteReg(0x51, x + x_end);             /* Horizontal GRAM End   Address (-1) */
        LCD_WriteReg(0x52, y);                      /* Vertical   GRAM Start Address      */
        LCD_WriteReg(0x53, y + y_end);              /* Vertical   GRAM End   Address (-1) */
    }
    drv_lcdSetCursor(x, y);			//设置光标位置
}
/*****************************************************************************
** 函数名称: drv_lcdShowString
** 功能描述: 显示字符串函数
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
#define MAX_CHAR_POSX 232
#define MAX_CHAR_POSY 304
void drv_lcdShowString(UINT8 x, UINT16 y, UINT8 *p)
{
    while(*p != '\0')
    {
        if(x > MAX_CHAR_POSX)
        {
            x = 0;
            y += 16;
        }
        if(y > MAX_CHAR_POSY)
        {
            y = x = 0;
            drv_lcdClear(WHITE);
        }
        LCD_ShowChar(x, y, *p, 16, 0);
        x += 8;
        p++;
    }
}
/*****************************************************************************
** 函数名称: LCD_ShowChar
** 功能描述: 显示一个字符函数
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void LCD_ShowChar(UINT8 x, UINT16 y, UINT8 chars, UINT8 size, UINT8 mode)
{
    UINT8 temp;
    UINT8 pos, t;
    if(x > MAX_CHAR_POSX || y > MAX_CHAR_POSY)
    {
        return;
    }
    drv_lcdSetWindow(x, y, (size / 2 - 1), size - 1); //设置窗口
    drv_lcdWriteRAMPrepare();        //开始写入GRAM
    if(!mode) 						//非叠加方式
    {
        for(pos = 0; pos < size; pos++)
        {
            if(size == 12)
            {
                temp = ASCII_1206[chars - 0x20][pos];    //调用1206字体
            }
            else
            {
                temp = ASCII_1608[chars - 0x20][pos];    //调用1608字体
            }
            for(t = 0; t < size / 2; t++)
            {
                //if(temp&0x01)            	 			//先传低位，取模有关系
                if((temp << t) & 0x80)						//先传高位
                {
                    Write_Dat(RED);
                }
                else
                {
                    Write_Dat(WHITE);
                }
                //temp>>=1; 	   							//如果先传低位，去掉屏蔽线
            }
        }
    }
    else//叠加方式
    {
        for(pos = 0; pos < size; pos++)
        {
            if(size == 12)
            {
                temp = ASCII_1206[chars - 0x20][pos];    //调用1206字体
            }
            else
            {
                temp = ASCII_1608[chars - 0x20][pos];    //调用1608字体
            }
            for(t = 0; t < size / 2; t++)
            {
                if((temp << t) & 0x80)
                {
                    drv_lcdDrawPoint(x + t, y + pos);    //画一个点
                }
                //temp>>=1; 								//如果先传低位，去掉屏蔽线
            }
        }
    }
    /* 恢复窗体大小	*/
//	LCD_WriteReg(0x43, 0x0000); //水平方向GRAM起始地址
    LCD_WriteReg(0x44, 0xEF00); //水平方向GRAM结束地址
    LCD_WriteReg(0x45, 0x0000); //垂直方向GRAM起始地址
    LCD_WriteReg(0x46, 0x013F); //垂直方向GRAM结束地址
}
/*****************************************************************************
** 函数名称: drv_lcdClear
** 功能描述: 清屏幕函数
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void drv_lcdClear(UINT16 Color)
{
    UINT32 index = 0;
    drv_lcdSetCursor(0x00, 0x0000); //设置光标位置
    drv_lcdWriteRAMPrepare();     //开始写入GRAM
    for(index = 0; index < 76800; index++)
    {
        Write_Dat(Color);
    }
}
/*****************************************************************************
** 函数名称: drv_lcdWriteString
** 功能描述: 在指定位置开始显示一个字符串和一串汉字
				支持自动换行
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void drv_lcdWriteString(UINT16 x0, UINT16 y0, UINT8 *pcStr, UINT16 color)
{
    UINT16 usIndex;
    UINT16 usWidth = 0;
    FNT_GB16 *ptGb16 = 0;
    ptGb16 = (FNT_GB16 *)GBHZ_16;
    while(1)
    {
        if(*pcStr == 0)
        {
            break;                                     /* 字符串结束            */
        }
        x0 = x0 + (usWidth);                           /* 调节字符串显示松紧度         */
        if(*pcStr > 0x80)                              /* 判断为汉字                   */
        {
            if((x0 + 16) > LCD_W)                      /* 检查剩余空间是否足够         */
            {
                x0 = 0;
                y0 = y0 + 16;                          /* 改变显示坐标                 */
                if(y0 > LCD_H)                         /* 纵坐标超出                   */
                {
                    y0 = 0;
                }
            }
            usIndex = findHzIndex(pcStr);
            usWidth = drv_lcdWriteOneHzChar((UINT8 *) & (ptGb16[usIndex].Msk[0]), x0, y0, color);
            /* 显示字符                     */
            pcStr += 2;
        }
        else
        {
            /* 判断为非汉字                 */
            if(*pcStr == '\r')                          /* 换行                         */
            {
                y0 = y0 + 16;                           /* 改变显示坐标                 */
                if(y0 > LCD_H)                          /* 纵坐标超出                   */
                {
                    y0 = 0;
                }
                pcStr++;
                usWidth = 0;
                continue;
            }
            else if(*pcStr == '\n')                     /* 对齐到起点                   */
            {
                x0 = 0;
                pcStr++;
                usWidth = 0;
                continue;
            }
            else
            {
                if((x0 + 8) > LCD_W)                     /* 检查剩余空间是否足够         */
                {
                    x0 = 0;
                    y0 = y0 + 16;                        /* 改变显示坐标                 */
                    if(y0 > LCD_H)                       /* 纵坐标超出                   */
                    {
                        y0 = 0;
                    }
                }
                usWidth = drv_lcdWriteOneASCII((UINT8 *)&ASCII_1608[(*pcStr - 0x20)][0], x0, y0, color);
                /* ASCII码表21H的值对应区位码3区*/
                pcStr += 1;
            }
        }
    }
}
/*****************************************************************************
** 函数名称: drv_lcdWriteOneHzChar
** 功能描述: 显示一个指定大小的汉字
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
UINT16 drv_lcdWriteOneHzChar(UINT8 *pucMsk,
                             UINT16 x0,
                             UINT16 y0,
                             UINT16 color)
{
    UINT16 i, j;
    UINT16 mod[16];                                      /* 当前字模                     */
    UINT16 *pusMsk;                                      /* 当前字库地址                 */
    UINT16 y;
    pusMsk = (UINT16 *)pucMsk;
    for(i = 0; i < 16; i++)                                /* 保存当前汉字点阵式字模       */
    {
        mod[i] = *pusMsk++;                                /* 取得当前字模，半字对齐访问   */
        mod[i] = ((mod[i] & 0xff00) >> 8) | ((mod[i] & 0x00ff) << 8);/* 字模交换高低字节（为了显示   */
        /* 需要）                       */
    }
    y = y0;
    for(i = 0; i < 16; i++)                                /* 16行                         */
    {
#ifdef __DISPLAY_BUFFER                            /* 使用显存显示                 */
        for(j = 0; j < 16; j++)                            /* 16列                         */
        {
            if((mod[i] << j) & 0x8000)                     /* 显示字模                     */
            {
                DispBuf[240 * (y0 + i) + x0 + j] = color;
            }
        }
#else                                              /* 直接显示                     */
        drv_lcdSetCursor(x0, y);                              /* 设置写数据地址指针           */
        drv_lcdWriteRAMPrepare();        					   /*开始写入GRAM	*/
        for(j = 0; j < 16; j++)                            /* 16列                         */
        {
            if((mod[i] << j) & 0x8000)                     /* 显示字模                     */
            {
                Write_Dat(color);
            }
            else
            {
                Write_Dat(BACK_COLOR);                     /* 用读方式跳过写空白点的像素   */
                //LCD_ReadDat();
            }
        }
        y++;
#endif
    }
    return (16);                                          /* 返回16位列宽                 */
}
/*****************************************************************************
** 函数名称: drv_lcdWriteOneASCII
** 功能描述: 显示一个指定大小的字符
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
UINT16 drv_lcdWriteOneASCII(UINT8 *pucMsk,
                            UINT16 x0,
                            UINT16 y0,
                            UINT16 color)
{
    UINT16 i, j;
    UINT16 y;
    UINT8 ucChar;
    y = y0;
    for(i = 0; i < 16; i++)                               /* 16行                         */
    {
        ucChar = *pucMsk++;
#ifdef __DISPLAY_BUFFER                           /* 使用显存显示                 */
        for(j = 0; j < 8; j++)                            /* 8列                          */
        {
            if((ucChar << j) & 0x80)                      /* 显示字模                     */
            {
                DispBuf[240 * (y0 + i) + x0 + j] = color;
            }
        }
#else                                             /* 直接显示                     */
        drv_lcdSetCursor(x0, y);                             /* 设置写数据地址指针           */
        drv_lcdWriteRAMPrepare();        					  /* 开始写入GRAM	    		  */
        for(j = 0; j < 8; j++)                            /* 8列                          */
        {
            if((ucChar << j) & 0x80)                      /* 显示字模                     */
            {
                Write_Dat(color);
            }
            else
            {
                Write_Dat(BACK_COLOR);
                //LCD_ReadDat();
            }
        }
        y++;
#endif
    }
    return (8);                                           /* 返回16位列宽                 */
}
/*****************************************************************************
** 函数名称: Num_power
** 功能描述: M的N次方
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
UINT32 Num_power(UINT8 m, UINT8 n)
{
    UINT32 result = 1;
    while(n--)
    {
        result *= m;
    }
    return result;
}
/*****************************************************************************
** 函数名称: LCD_ShowNum
** 功能描述: 在指定位置显示一串数字
				num:数值(0~4294967295);
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void LCD_ShowNum(UINT8 x, UINT16 y, UINT32 num, UINT8 len, UINT8 size)
{
    UINT8 t, temp;
    UINT8 enshow = 0;
    for(t = 0; t < len; t++)
    {
        temp = (num / Num_power(10, len - t - 1)) % 10;
        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                LCD_ShowChar(x + (size / 2)*t, y, ' ', size, 0);
                continue;
            }
            else
            {
                enshow = 1;
            }
        }
        LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size, 0);
    }
}
/*****************************************************************************
** 函数名称: drv_lcdShowPicture
** 功能描述: 在指定的位置显示一幅图片
				Xpos和Ypos为图片显示地址，Height和Width 为图片的宽度和高度
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void drv_lcdShowPicture(UINT8 Xpos, UINT16 Ypos, UINT8 Height, UINT16 Width, UINT8 *bitmap)
{
    UINT32 index;
    UINT32 size = Height * Width;
    UINT16 *bitmap_ptr = (UINT16 *)bitmap;
    drv_lcdSetWindow(Xpos, Ypos, Width - 1, Height - 1);
    //LCD_WriteReg(0x03, 0x1038);	//如果需要横向显示图片，将此去掉屏蔽 ，同时将Width和Hight调换一下就可以
    drv_lcdWriteRAMPrepare();
    for(index = 0; index < size; index++)
    {
        Write_Dat(*bitmap_ptr++);
    }
    //恢复窗体大小
    LCD_WriteReg(R80, 0x0000); //水平方向GRAM起始地址
    LCD_WriteReg(R81, 0x00EF); //水平方向GRAM结束地址
    LCD_WriteReg(R82, 0x0000); //垂直方向GRAM起始地址
    LCD_WriteReg(R83, 0x013F); //垂直方向GRAM结束地址
}
/*****************************************************************************
** 函数名称: drv_lcdDrawLine
** 功能描述: 在指定的两侧位置画一条线
				x1,y1:起点坐标度  x2,y2:终点坐标
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void drv_lcdDrawLine(UINT16 x1, UINT16 y1, UINT16 x2, UINT16 y2)
{
    UINT16 t;
    SINT16 xerr = 0, yerr = 0, delta_x, delta_y, distance;
    SINT16 incx, incy, uRow, uCol;
    delta_x = x2 - x1; 				//计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if(delta_x > 0)
    {
        incx = 1;    //设置单步方向
    }
    else if(delta_x == 0)
    {
        incx = 0;    //垂直线
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if(delta_y > 0)
    {
        incy = 1;
    }
    else if(delta_y == 0)
    {
        incy = 0;    //水平线
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if(delta_x > delta_y)
    {
        distance = delta_x;    //选取基本增量坐标轴
    }
    else
    {
        distance = delta_y;
    }
    for(t = 0; t <= distance + 1; t++)	//画线输出
    {
        drv_lcdDrawPoint(uRow, uCol); //画点
        xerr += delta_x ;
        yerr += delta_y ;
        if(xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if(yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}
/*****************************************************************************
** 函数名称: drv_lcdDrawLine
** 功能描述: 在指定位置画一个指定大小的圆
				(x,y):中心点 	 r    :半径
** 作  　者: Dream
** 日　  期: 2010年12月06日
*****************************************************************************/
void drv_lcdDrawCircle(UINT8 x0, UINT16 y0, UINT8 r)
{
    SINT16 a, b;
    SINT16 di;
    a = 0;
    b = r;
    di = 3 - (r << 1);       //判断下个点位置的标志
    while(a <= b)
    {
        drv_lcdDrawPoint(x0 - b, y0 - a);        //3
        drv_lcdDrawPoint(x0 + b, y0 - a);        //0
        drv_lcdDrawPoint(x0 - a, y0 + b);        //1
        drv_lcdDrawPoint(x0 - b, y0 - a);        //7
        drv_lcdDrawPoint(x0 - a, y0 - b);        //2
        drv_lcdDrawPoint(x0 + b, y0 + a);        //4
        drv_lcdDrawPoint(x0 + a, y0 - b);        //5
        drv_lcdDrawPoint(x0 + a, y0 + b);        //6
        drv_lcdDrawPoint(x0 - b, y0 + a);
        a++;
        //使用Bresenham算法画圆
        if(di < 0)
        {
            di += 4 * a + 6;
        }
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
        drv_lcdDrawPoint(x0 + a, y0 + b);
    }
}

//画一个大点
//2*2的点
void drv_lcdDrawBigPoint(UINT16 x, UINT16 y)
{
    drv_lcdDrawPoint(x, y); //中心点
    drv_lcdDrawPoint(x + 1, y);
    drv_lcdDrawPoint(x, y + 1);
    drv_lcdDrawPoint(x + 1, y + 1);
}

/*********************************************************************************************************
** End of File
*********************************************************************************************************/
