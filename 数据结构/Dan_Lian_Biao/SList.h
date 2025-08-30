#include<assert.h>
#include<stdlib.h>
#include<stdio.h>
typedef int SLDataType;
typedef struct SLNode
{
    SLDataType data;   
    struct SLNode* next;
}SLNode;

//尾插
void SLPushBack(SLNode** pphead,SLDataType x);

//创建节点
SLNode* SLBuyNode(SLDataType x);

//尾删
void SLPopBack(SLNode** pphead);

//头插
void SLPushFront(SLNode** pphead,SLDataType x);

//头删
void SLPopFront(SLNode** pphead);

//输出
void SLPrint(SLNode* phead);

//查找
SLNode* SLFind(SLNode* phead,SLDataType x);

//pos前面插入
void SLInsert(SLNode** pphead,SLNode* pos,SLDataType x);

//pos位置删除
void SLErease(SLNode** pphead,SLNode* pos);

//pos后面插入
void SLInsertAfter(SLNode** pphead,SLNode* pos,SLDataType x);

//pos后面删除
void SLErease(SLNode** pphead,SLNode* pos);

