#include "Mylibrary.h"
#include "string.h"
#include "math.h"

//排序算法
void Quick_Sort(int *pData, int count)
{
    int i, j, k, x;
    k = count / 2;
    while(k >= 1)
    {
        for(i = k; i < count; i++)
        {
            x = pData[i];
            j = i - k;
            while(j >= 0 && x < pData[j])
            {
                pData[j + k] = pData[j];
                j -= k;
            }
            pData[j + k] = x;
        }
        k /= 2;
    }
}

void Bubble_Sort(int *pData, int count)
{
    int i, j, iTemp;
    for(i = count - 1; i > 0; i--)
    {
        for(j = 0; j < i; j++)
        {
            if(pData[j] > pData[j + 1])
            {
                iTemp = pData[j];
                pData[j] = pData[j + 1];
                pData[j + 1] = iTemp;
            }
        }
    }
}

void Select_Sort(int *pData, int count)
{
    int i, j, iTemp, iPos;
    for(i = 0; i < count - 1; i++)
    {
        iTemp = pData[i];
        iPos = i;
        for(j = i + 1; j < count; j++)
        {
            if(pData[j] < iTemp)
            {
                iTemp = pData[j];
                iPos = j;
            }
        }
        pData[iPos] = pData[i];
        pData[i] = iTemp;
    }
}

void Insert_Sort(int *pData, int count)
{
    int i, iTemp, iPos;
    for(i = 1; i < count; i++)
    {
        iTemp = pData[i];
        iPos = i - 1;
        while((iPos >= 0) && (iTemp < pData[iPos]))
        {
            pData[iPos + 1] = pData[iPos];
            iPos--;
        }
        pData[iPos + 1] = iTemp;
    }
}

//滤波算法
//去最值平均滤波
int RemoveExtremumAverage(int *pData, int count, void (*AverageWay)(), int RemoveNum)
{
    int i, end;
    long Tmp = 0;
    end = count - RemoveNum;
    AverageWay(pData, count);
    for(i = RemoveNum; i < end; i++)
    {
        Tmp += pData[i];
    }
    return Tmp / (count - 2 * RemoveNum);
}
//递推平均滤波
int RecursiveAverageSort(QueueAverage *queue)
{
    int count;
    long sum = 0;
    queue->Data[queue->InsertPlace++] = queue->NewData;
    if(queue->InsertPlace == queue->QueueLength)
    {
        queue->InsertPlace = 0;
    }
    for(count = 0; count < queue->QueueLength; count++)
    {
        sum += queue->Data[count];
    }
    return (int)(sum / queue->QueueLength);
}
#if 0
/******************************************************
*函数名称:CRC16RTU
*输   入:pszBuf  要校验的数据
        unLength 校验数据的长
*输   出:校验值
*功   能:循环冗余校验-16
         （RTU标准-0xA001）
*******************************************************/
unsigned short CRC16RTU(unsigned char *pszBuf, unsigned short unLength)
{
    unsigned short CRC = 0XFFFF;
    unsigned short CRC_count;
    for(CRC_count = 0; CRC_count < unLength; CRC_count++)
    {
        int i;
        CRC = CRC ^ *(pszBuf + CRC_count);
        for(i = 0; i < 8; i++)
        {
            if(CRC & 1)
            {
                CRC >>= 1;
                CRC ^= 0xA001;
            }
            else
            {
                CRC >>= 1;
            }
        }
    }
    return CRC;
}
#endif

