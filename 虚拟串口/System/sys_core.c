#include "sys.h"

STASK_STRUCT st_Task[MAX_TASK];

void sys_taskInit(void)
{
    SYS_UINT8 i;
#ifdef SYS_TASK_WATCHDOG
    sys_taskWatchDogInit();
#endif
    for(i = 0; i < MAX_TASK; i++)
    {
        sys_taskDel(i);
    }
    sys_clockInit();
}

void sys_taskAdd(void (*ptask)(void), SYS_UINT period, SYS_UINT8 prio)
{
    assert_param(prio < MAX_TASK);
    if(st_Task[prio].ptask == SYS_NULL)
    {
        st_Task[prio].ptask = ptask;
        st_Task[prio].dly = 0;
        st_Task[prio].period = period;
        st_Task[prio].state = TASK_WAIT_RUN;
    }
}

void sys_taskScheduler(void)
{
    SYS_UINT8 i;
    for(i = 0; i < MAX_TASK; i++)
    {
        if(st_Task[i].state == TASK_WAIT_RUN && st_Task[i].ptask != SYS_NULL)
        {
            if(st_Task[i].dly == 0)
            {
                st_Task[i].state = TASK_RUN;
                st_Task[i].dly = st_Task[i].period;
            }
            st_Task[i].dly--;
        }
    }
}

void sys_taskStart(void)
{
    SYS_UINT8 i;
    while(1)
    {
        for(i = 0; i < MAX_TASK; i++)
        {
            if(st_Task[i].state == TASK_RUN)
            {
                (*st_Task[i].ptask)();
                st_Task[i].state = TASK_WAIT_RUN;
                if(st_Task[i].period == 0)
                {
                    sys_taskDel(i);
                }
                break;
            }
        }
#ifdef SYS_TASK_STATE
        sys_taskState();
#endif
#ifdef SYS_TASK_WATCHDOG
        sys_taskWatchDog();
#endif
#ifdef SYS_TASK_SLEEP
        sys_taskSleep();
#endif
    }
}

void sys_taskDel(SYS_UINT8 prio)
{
    assert_param(prio < MAX_TASK);
    st_Task[prio].ptask = SYS_NULL;
    st_Task[prio].dly = 0;
    st_Task[prio].period = 0;
    st_Task[prio].state = TASK_DEL;
}

#ifdef SYS_TASK_PEND
void sys_taskPend(SYS_UINT8 prio)
{
    assert_param(prio < MAX_TASK);
    st_Task[prio].state = TASK_PEND;
}
#endif

#ifdef SYS_TASK_PESUME
void sys_taskResume(SYS_UINT8 prio)
{
    assert_param(prio < MAX_TASK);
    st_Task[prio].state = TASK_WAIT_RUN;
}
#endif

#ifdef SYS_TASK_PRIO
SYS_UINT8 sys_taskPrio(void (*ptask)(void))
{
    SYS_UINT8 prio;
    for(prio = 0; prio < MAX_TASK; prio++)
    {
        if(st_Task[prio].ptask == ptask)
        {
            if(st_Task[prio].dly == 0)
            {
                break;
            }
        }
    }
    assert_param(prio < MAX_TASK);
    return prio;
}
#endif

#ifdef SYS_TASK_STATE
void sys_taskState(void)
{
}
#endif

