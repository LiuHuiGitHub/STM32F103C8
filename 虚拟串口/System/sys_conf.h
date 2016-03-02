#ifndef __SYS_CONF_H__
#define __SYS_CONF_H__

/*typedef*/
typedef unsigned char				SYS_UINT8;
typedef unsigned short			SYS_UINT16;
typedef unsigned long				SYS_UINT32;

/*define*/
#define SYS_NULL						((void *)0L)

/*config*/
#define MAX_TASK						(10u)
#define TASK_CLOCK_CYCLE		(1u)    //ms

#define SYS_TASK_PRIO
#define SYS_TASK_PEND
#define SYS_TASK_PESUME
//#define SYS_TASK_STATE
#define SYS_TASK_WATCHDOG
//#define SYS_TASK_SLEEP

#define PERIOD_16BIT
//#define USE_FULL_ASSERT

#ifdef PERIOD_16BIT
#define SYS_UINT				SYS_UINT16
#else
#define SYS_UINT				SYS_UINT8
#endif

#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports
  *         the name of the source file and the source line number of the call
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t *file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif
