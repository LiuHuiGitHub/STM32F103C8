#include "uLib_queue.h"
#include "string.h"

#define IS_QUEUE_EMPTY(__q)       (__q->Rear)
#define IS_QUEUE_FULL(__q)        (0)

void uLib_QueueCreate(LIB_QUEUE *q, void *buff, UINT32 member_size, UINT32 queue_size)
{
    q->Front = 0;
    q->Rear = 0;
    q->pBase = buff;
    q->Size = queue_size;
    q->MemberSize = member_size;
}

BOOL QueueEmpty(LIB_QUEUE *q)
{
    if(q->Front == q->Rear)
    {
        return TRUE;
    }
    return FALSE;
}

BOOL QueueFull(LIB_QUEUE *q)
{
    if(q->Front == (q->Rear + q->MemberSize) % q->Size)
    {
        return TRUE;
    }
    return FALSE;
}

BOOL uLib_QueueInsert(LIB_QUEUE *q, void *member)
{
    if(QueueFull(q))
    {
        return FALSE;
    }
    memcpy((UINT8 *)q->pBase + q->Rear, member, q->MemberSize);
    q->Rear = (q->Rear + q->MemberSize) % q->Size;
    return TRUE;
}

BOOL uLib_QueueFirst(LIB_QUEUE *q, void *member)
{
    if(QueueEmpty(q))
    {
        return FALSE;
    }
    memcpy(member, (UINT8 *)q->pBase + q->Front, q->MemberSize);
    q->Front = (q->Front + q->MemberSize) % q->Size;
    return TRUE;
}

UINT32 uLib_QueueOut(LIB_QUEUE *q, void *buff, UINT32 memberNum)
{
    UINT32 i;
    for(i = 0; i < memberNum; i++)
    {
        if(uLib_QueueFirst(q, (UINT8 *)buff + i * q->MemberSize) == FALSE)
        {
            break;
        }
    }
    return i;
}

UINT32 uLib_QueueIn(LIB_QUEUE *q, void *buff, UINT32 memberNum)
{
    UINT32 i;
    for(i = 0; i < memberNum; i++)
    {
        if(uLib_QueueInsert(q, (UINT8 *)buff + i * q->MemberSize) == FALSE)
        {
            break;
        }
    }
    return i;
}

