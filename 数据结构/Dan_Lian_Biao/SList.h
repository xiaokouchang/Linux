#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
typedef int SLDataType;
typedef struct SLNode
{
    SLDataType data;
    struct SLNode* next;
}SLNode;

//尾插
void SLPushBack(SLNode** pphead,SLDataType x);

//头插
void SLPushFront(SLNode** pphead,SLDataType x);

//尾删
void SLPopBack(SLNode** pphead);

//头删
void SLPopFront(SLNode** pphead);

//输出
void SLPrint(SLNode* phead);

//新建节点
SLNode* SLBuyNode(SLDataType x);

//销毁
void SLDestory1(SLNode* phead);
void SLDestory2(SLNode** pphead);

//查找
SLNode* SLFind(SLNode* phead,SLDataType x);

//pos之前插入
void SLInsertFront(SLNode** pphead,SLNode* pos,SLDataType x);

//删除
void SLErease(SLNode** pphead,SLNode* pos);

//pos之后插入
void SLInsertAfter(SLNode** pphead,SLNode* pos,SLDataType x);

//删除
void SLEreaseAfter(SLNode** pphead,SLNode* pos);



