#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
typedef int SLDataType;
#define Init_Capacity 4
#define Init_Size 2

typedef struct SeqList
{
    SLDataType* a;
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
void SLPushBack(SL* ps,SLDataType x);

//头插
void SLPushFront(SL*ps,SLDataType x);

//尾删
void SLPopBack(SL* ps);

//头删
void SLPopFront(SL* ps);

//输出
void SLPrint(SL* ps);

//插入
void SLInsert(SL* ps,int pos,SLDataType x);

//删除
void SLErease(SL* ps,int pos);

//查找
int SLFind(SL* ps,SLDataType x);
