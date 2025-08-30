#include"SList.h"
//尾插
void SLPushBack(SLNode** pphead,SLDataType x)
{
    assert(pphead);
    SLNode* newnode = SLBuyNode(x);
    if(*pphead == NULL)
    {
        *pphead=newnode;
    }
    else 
    {
        SLNode* cur = *pphead;
        while(cur->next)
        {
            cur = cur->next;
        }
        cur->next=newnode;
    }
}

//创建节点
SLNode* SLBuyNode(SLDataType x)
{
    SLNode* newnode = (SLNode*)malloc(sizeof(SLNode));
    if(newnode == NULL)
    {
        perror("malloc fail");
        return NULL;
    }
    newnode->data=x;
    newnode->next=NULL;
    return newnode;
}

//尾删
void SLPopBack(SLNode** pphead)
{
    assert(*pphead);
    assert(pphead);
    if((*pphead)->next==NULL)
    {
        free(*pphead);
        *pphead=NULL;
    }
    else 
    {
        SLNode* cur = *pphead;
        SLNode* prev = NULL;
        while(cur->next)
        {
            prev = cur;
            cur = cur->next;
        }
        //prev cur 
        prev->next=NULL;
        free(cur);
        cur=NULL;
    }
}

//头插
void SLPushFront(SLNode** pphead,SLDataType x)
{
    assert(pphead);
    SLNode* newnode = SLBuyNode(x);
    newnode->next = *pphead;
    *pphead = newnode;
}

//头删
void SLPopFront(SLNode** pphead)
{
    assert(pphead);
    assert(*pphead);
    SLNode* temp = *pphead;
    *pphead = temp->next;
    free(temp);
    temp=NULL;
}

//输出
void SLPrint(SLNode* phead)
{
    assert(phead);
    SLNode* cur = phead;
    while(cur)
    {
        printf("%d->",cur->data);
        cur=cur->next;
    }
    printf("NULL\n");
}

//查找
SLNode* SLFind(SLNode* phead,SLDataType x)
{
    assert(phead);
    SLNode* cur = phead;
    while(cur)
    {
        if(cur->data == x)
        {
            return cur;
        }
        cur=cur->next;
    }
    return NULL;
}

//pos前面插入
void SLInsert(SLNode** pphead,SLNode* pos,SLDataType x)
{
    assert(pos);
    assert(pphead);
    SLNode* newnode = SLBuyNode(x);
    if(*pphead == NULL)
    {
        SLPushFront(pphead,x);
    }
    else 
    {
        SLNode* cur = *pphead;
        while(cur->next!=pos)
        {
            cur=cur->next;
        }
        //prev pos 
        cur->next = newnode;
        newnode->next = pos;
    }
}

//pos位置删除
void SLErease(SLNode** pphead,SLNode* pos)
{
    assert(pos);
    assert(pphead);
    if(pos==*pphead)
    {
        SLPopFront(pphead);
    }
    else 
    {
        SLNode* cur = *pphead;
        while(cur->next != pos)
        {
            cur = cur->next;
        }
        //cur pos 
        cur->next = pos->next;
        free(pos);
        pos=NULL;
    }
}

//pos后面插入
void SLInsertAfter(SLNode* pos,SLDataType x)
{
    assert(pos);
    SLNode* newnode = SLBuyNode(x);
    //pos 
    newnode->next = pos->next;
    pos->next =newnode;
}

//pos后面删除
void SLErease(SLNode* pos)
{
    assert(pos);
    SLNode* cur = pos->next;
    pos->next = cur->next;
    free(cur);
    cur=NULL;
}

