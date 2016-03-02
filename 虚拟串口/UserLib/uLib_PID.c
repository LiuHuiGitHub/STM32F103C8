#include "uLib_PID.h"
#include "math.h"

PID_STRUCT PID;

void uLib_PID_Init(void)
{
    PID.Kp = 0.2f;
    PID.Ki = 0.01f;
    PID.Kd = 0.1;
    PID.Err = 0;
    PID.ErrLast = 0;
    PID.value = 0;
    PID.NowValue = 0;
    PID.Integral = 0;
}
float uLib_PID_Cal(float HopeValue)
{
    PID.Err = HopeValue - PID.NowValue;
    PID.Integral += PID.Err;
    PID.value += PID.Kp * PID.Err + PID.Ki * PID.Integral + PID.Kd * (PID.Err - PID.ErrLast);
    if(PID.value > 100)
    {
        PID.value = 100;
    }
    PID.ErrLast = PID.Err;
    return PID.value;
}

