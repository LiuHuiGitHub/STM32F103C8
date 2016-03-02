#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__


typedef unsigned char		BOOL;

typedef unsigned char       UINT8;
typedef signed char         SINT8;
typedef unsigned short      UINT16;
typedef signed short		SINT16;
typedef unsigned int		UINT32;
typedef signed int			SINT32;

#ifndef TRUE
#define TRUE		(1u)
#endif
#ifndef FALSE
#define FALSE		(0u)
#endif

//#ifndef ENABLE
//#define ENABLE	1u
//#endif
//#ifndef DISABLE
//#define DISABLE	0u
//#endif

#define BIT0    (0x01)
#define BIT1    (0x02)
#define BIT2    (0x04)
#define BIT3    (0x08)
#define BIT4    (0x10)
#define BIT5    (0x20)
#define BIT6    (0x40)
#define BIT7    (0x80)
#define BIT8    (0x0100)
#define BIT9    (0x0200)
#define BIT10   (0x0400)
#define BIT11   (0x0800)
#define BIT12   (0x1000)
#define BIT13   (0x2000)
#define BIT14   (0x4000)
#define BIT15   (0x8000)

#define countof(a)				(sizeof(a)/sizeof(*(a)))

/*位带操作,实现51类似的GPIO控制功能*/
/*IO口操作宏定义 */
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+ \
                               0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8)  //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8)  //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8)  //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8)  //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8)  //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8)  //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8)  //0x40011E08 

//IO口操作,只对单一的IO口!    //0<=n<=15!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#endif
