#include "stm32f10x.h"
#include "sys_watchDog.h"

/*¶ÀÁ¢¿´ÃÅ¹·*/
void sys_watchDogInit(void)
{
    RCC_LSICmd(ENABLE);
    while(!RCC_GetFlagStatus(RCC_FLAG_LSIRDY));
    IWDG->KR = 0x5555;
    IWDG->PR |= IWDG_Prescale;
    IWDG->RLR = IWDG_Reset_Time * 40 / (1 << (IWDG_Prescale + 2));
    IWDG->KR = 0xCCCC;
    IWDG->KR = 0xAAAA;
}

void sys_watchDogReset(void)
{
    IWDG->KR = 0xAAAA;
}
