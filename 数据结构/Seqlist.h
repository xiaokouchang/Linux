#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#define Init_Capacity 4
typedef int DataType;
typedef struct Seqlist
{
    DataType* a;
    int size;
    int capacity;
}SL;

//初始化
void SLInit(SL* ps);

//销毁
void SLDestory(SL* ps);

//扩容
void SLCheckCapacity(SL* ps);

//尾插
void SLPushBack(SL* ps,DataType x);

//尾删
void SLPopBack(SL* ps);

//头插
void SLPushFront(SL* ps,DataType x);

//头删
void SLPopFront(SL* ps);

//输出
void SLPrint(SL* ps);


