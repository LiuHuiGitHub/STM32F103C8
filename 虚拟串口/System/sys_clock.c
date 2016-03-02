#include "sys.h"
#include "sys_count.h"

void sys_clockInit(void)
{
    sys_timeInit();
    sys_countInit();
}

void sys_clockScheduler(void)
{
//	sys_countAddCnt();
}
