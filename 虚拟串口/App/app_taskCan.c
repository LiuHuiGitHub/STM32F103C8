#include "app_can.h"
#include "app_taskCan.h"


void app_taskCanHandler10ms(void)
{
    UINT8 i;
    static UINT8 TimeCnt1s = 0;
    static UINT8 TimeCnt = 0;
    static UINT8 Num = 0;
    if(++TimeCnt1s >= 50)
    {
        TimeCnt1s = 0;
        app_canCircle(EMS_EngineSpeed, Num);
        app_canLight(ESP_VehicleSpeed_Enable, 1);
        app_canCircle(ESP_VehicleSpeed, Num);
        app_canCircle(EMS_EngineCoolanTemperature, Num);
        Num++;
        if(Num >= 100)
            Num = 0;
    }
    if(++TimeCnt == 1)
    {
        for(i = 0; i < NUM_OF_LIGHT_NUM - 1; i++)
        {
            app_canLight((Light_ENUM)i, 1);
        }
    }
    else if(TimeCnt == 50)
    {
        for(i = 0; i < NUM_OF_LIGHT_NUM - 1; i++)
        {
            app_canLight((Light_ENUM)i, 0);
        }
    }
    else if(TimeCnt == 100)
    {
        app_canCircle(EMS_EngineSpeed, 0);
        app_canCircle(ESP_VehicleSpeed, 0);
    }
    else if(TimeCnt == 150)
    {
        TimeCnt = 0;
    }
}
