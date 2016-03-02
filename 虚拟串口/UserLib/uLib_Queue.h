#ifndef __USER_LIB_QUEUE_H__
#define __USER_LIB_QUEUE_H__

#include "string.h"
#include "typedef.h"

typedef struct
{
    void *pBase;
    UINT32 Front;
    UINT32 Rear;
    UINT32 Size;
    UINT32 MemberSize;
    UINT32 MemberNum;
} LIB_QUEUE;

void uLib_QueueCreate(LIB_QUEUE *q, void *buff, UINT32 member_size, UINT32 queue_size);
BOOL uLib_QueueInsert(LIB_QUEUE *q, void *member);
BOOL uLib_QueueFirst(LIB_QUEUE *q, void *member);
UINT32 uLib_QueueOut(LIB_QUEUE *q, void *buff, UINT32 memberNum);
UINT32 uLib_QueueIn(LIB_QUEUE *q, void *buff, UINT32 memberNum);


#endif
