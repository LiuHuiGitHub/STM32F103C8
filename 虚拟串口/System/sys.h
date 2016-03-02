#ifndef __SYS_H__
#define __SYS_H__

#include "typedef.h"
#include "sys_conf.h"

typedef enum
{
    TASK_DEL	= 0u,
    TASK_PEND,
    TASK_WAIT_RUN,
    TASK_WAIT_EVENT,
    TASK_RUN,
} RUN_ENUM;

typedef struct
{
    void (*ptask)(void);
    SYS_UINT dly;
    SYS_UINT period;
    RUN_ENUM state;
} STASK_STRUCT;


/*sys_core.c*/
void sys_taskInit(void);
void sys_taskAdd(void (*ptask)(void), SYS_UINT period, SYS_UINT8 prio);
void sys_taskDel(SYS_UINT8 prio);
void sys_taskScheduler(void);
void sys_taskStart(void);

#ifdef SYS_TASK_PRIO
SYS_UINT8 sys_taskPrio(void (*ptask)(void));
#endif
#ifdef SYS_TASK_PEND
void sys_taskPend(SYS_UINT8 prio);
#endif
#ifdef SYS_TASK_PESUME
void sys_taskResume(SYS_UINT8 prio);
#endif
#ifdef SYS_TASK_STATE
void sys_taskState(void);
#endif


/*sys_clock.c*/
void sys_clockInit(void);
void sys_clockScheduler(void);


/*sys_cpu.c*/
void sys_timeInit(void);
UINT16 sys_timeTick(void);
#ifdef SYS_TASK_WATCHDOG
void sys_taskWatchDogInit(void);
void sys_taskWatchDog(void);
#endif
#ifdef SYS_TASK_SLEEP
void sys_taskSleep(void);
#endif


#endif
