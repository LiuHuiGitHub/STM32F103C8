#include "system.h"
#include "hwa_key.h"

const BOOL KEY_Valid[KEY_NUM] = {1, 0, 0};
volatile KEY_STRUCT KEY_K[KEY_NUM];

BOOL hwa_getKEY(UINT8 key)
{
    if(key == 0)
    {
        return KEY_WKUP;
    }
    else if(key == 1)
    {
        return KEY_S2;
    }
    else
    {
        return KEY_S3;
    }
}

void hwa_keyInit(void)
{
    UINT8 i;
    for(i = 0; i < KEY_NUM; i++)
    {
        KEY_K[i].Flags.state 	= KEY_UP;
        KEY_K[i].Flags.level 	= hwa_getKEY(i);
        KEY_K[i].Flags.trig = 0;
        KEY_K[i].Flags.init  	= TRUE;
        KEY_K[i].TimeDlyCnt 	= 0;
        KEY_K[i].TimeCnt			= 0;
        KEY_K[i].Flags.unlash	= TRUE;
    }
}

void hwa_keyHandler10ms(void)
{
    UINT8 i = 0;
    for(i = 0; i < KEY_NUM; i++)
    {
        if(KEY_K[i].Flags.init == FALSE)
        {
            hwa_keyInit();
            return ;
        }
        KEY_K[i].Flags.level = hwa_getKEY(i);
        if(KEY_K[i].Flags.level != KEY_Valid[i])
        {
            if(KEY_K[i].TimeCnt && KEY_K[i].TimeCnt < TUNK_MIN_TIME)
            {
                KEY_K[i].Flags.trig = TRUE;
            }
            KEY_K[i].TimeCnt = 0;
            KEY_K[i].TimeDlyCnt = 0;
            KEY_K[i].Flags.state = KEY_UP;
            KEY_K[i].Flags.unlash = TRUE;
        }
        else if(KEY_K[i].Flags.unlash == TRUE)
        {
            if(KEY_K[i].TimeDlyCnt < DLY_TIME)
            {
                KEY_K[i].TimeDlyCnt++;
            }
            else
            {
                KEY_K[i].Flags.state = KEY_DOWN;
                KEY_K[i].Flags.unlash = FALSE;
            }
        }
        else if(KEY_K[i].TimeCnt < 0xFF)
        {
            KEY_K[i].TimeCnt++;
        }
    }
}


