#ifndef __USER_LIB_PID_H__
#define __USER_LIB_PID_H__

#ifndef __TYPEDEF_H__
typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned int        UINT32;
#endif


typedef struct
{
    float Kp, Ki, Kd;
    float Err, ErrLast;		//偏差
    float NowValue;			//当前反馈值
    float value;            //目标值
    float Integral;			//积分
} PID_STRUCT;

extern PID_STRUCT PID;

void uLib_PID_Init(void);
float uLib_PID_Cal(float HopeValue);

#endif
