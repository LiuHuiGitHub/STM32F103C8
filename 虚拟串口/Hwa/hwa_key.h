#ifndef __HWA_KEY_H__
#define __HWA_KEY_H__

/*explame
if(KEY_K[0].Flags.state == KEY_DOWN)
{
	KEY_K[0].Flags.state = KEY_UP;
	LED1 = !LED1;
}
if(KEY_K[0].Flags.trig == TRUE)
{
	KEY_K[0].Flags.trig = FALSE;
	LED1 = 0;
	sprintf((char*)s, "%d\0", (int)i++);
	drv_lcdShowString(0, 0, s);
}
*/
typedef struct
{
    BOOL state			: 1;
    BOOL level			: 1;
    BOOL trig				: 1;
    BOOL unlash 		: 1;
    BOOL init				: 1;
} KEY_flag;

typedef struct
{
    KEY_flag Flags;				//按键标志组
    UINT8 TimeDlyCnt;			//延时时间计数
    UINT8 TimeCnt;				//状态转换时间计数
} KEY_STRUCT;

//#ifndef TRUE
//#define TRUE		1u
//#endif
//#ifndef FALSE
//#define FALSE		0u
//#endif

#define KEY_DOWN					1
#define KEY_UP						0

#define KEY_NUM						3
#define DLY_TIME					5				//50ms
#define TUNK_MIN_TIME			50			//500ms		短按时间范围

extern volatile KEY_STRUCT KEY_K[KEY_NUM];

void hwa_keyInit(void);
void hwa_keyHandler10ms(void);

#endif

