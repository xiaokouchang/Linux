#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
typedef int SLDataType;
#define Init_Capacity 4
#define Check_Capacity 2
typedef struct Seqlist
{
    SLDataType* a; //数组
    int capacity;  //数组的容量
    int size;      //数组的大小
}SL;

//初始化顺序表
void SLInit(SL* ps);

//销毁
void SLDestory(SL* ps);

//扩容
void SLCheckCapacity(SL* ps);

//头删
void SLPopFront(SL* ps);

//尾删
void SLPopBack(SL* ps);

//头插
void SLPushFront(SL* ps, SLDataType x);

//尾插
void SLPushBack(SL* ps,SLDataType x);

//pos前面插入
void SLInsertPos(SL* ps,int pos,SLDataType x);

//pos位置删除
void SLErease(SL* ps,int pos);

//输出
void SLPrint(SL* ps);

//查找
int SLFind(SL* ps,SLDataType x);
