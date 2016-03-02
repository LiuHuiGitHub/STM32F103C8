#ifndef __MYLIBRARY_H__
#define __MYLIBRARY_H__


#define PRINT_EN              1                                                //1使能打印信息；0除能打印信息
//以下仅C99标准支持
#if PRINT_EN > 0
#define printu(...)  printf(__VA_ARGS__)                                    //普通信息打印
#define printe(...)  {printu("Error File:%s, Function:%s, Line%d:", __FILE__, __FUNCTION__, __LINE__); printu(__VA_ARGS__);}   //程序错误信息																												 
#else
#define printu(...)
#define printe(...)
#endif


void Quick_Sort(int *pData, int count); //快速排序
void Bubble_Sort(int *pData, int count); //冒泡排序
void Select_Sort(int *pData, int count); //选择排序
void Insert_Sort(int *pData, int count); //插入排序
//unsigned short CRC16RTU(unsigned char *pszBuf, unsigned short unLength);

int RemoveExtremumAverage(int *pData, int count, void (*AverageWay)(), int RemoveNum);//去最值平均滤波
typedef struct
{
    int *Data;
    int NewData;
    int QueueLength;
    int InsertPlace;
} QueueAverage;
/*使用方法
	static int ValueBuf[6];
	static QueueAverage Q={ValueBuf, 0, 6, 0};
	Q.NewData = ReadADCAverageValue(0x00);
	temp = RecursiveAverage(&Q);
*/
int RecursiveAverageSort(QueueAverage *queue);//递推平均滤波

#endif
