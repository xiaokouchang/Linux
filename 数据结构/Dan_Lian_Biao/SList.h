#include<stdio.h>
#include <assert.h>
#include<stdlib.h>
tydepef int SLDataType;

typedef struct SLNode
{
    DataType data;
    struct SLNode* next;
}SLNode;
//尾插
void SLPushBack(SLNode** pphead,SLDataType x);

//头插
void SLPushFront(SLNode** pphead,SLDataType x);

//插入节点
SLNode* SLBuyNode(SLDataType x);

//尾删
void SLPopBack(SLNode** pphead);

//头删
void SLPopFront(SLNode** pphead);

//查找
SLNode* SLFind(SLNode* phead,SLDataType x);

//插入
void SLInsert(SLNode** pphead,SLNode* pos,SLDataType x);

//删除
void SLErease(SLNode** pphead,SLNode* pos);

//删除
void SLEreaseAfter(SLNode** pphead,SLNode* pos);

//输出
void SLPrint(SLNode* phead);

