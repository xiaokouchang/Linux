#include"SList.h"
void SLPushBack(SLNode** pphead,SLDataType x)
{
    assert(pphead);
    SLNode* newnode = SLBuyNode(x);
    if(*pphead == NULL)
    {
        *pphead = newnode;
    }
    else 
    {
        SLNode* cur = *pphead;
        while(cur->next!=NULL)
        {
            cur = cur->next;
        }
        cur->next=newnode;
    }
}
//头插
void SLPushFront(SLNode** pphead,SLDataType x)
{
    assert(pphead);
    //newnode *pphead;
    SLNode* newnode = SLBuyNode(x);
    newnode->next = *pphead;
    *pphead = newnode;
}


//插入节点
SLNode* SLBuyNode(SLDataType x)
{
    SLNode* newnode = (SLNode*)malloc(sizeof(SLNode));
    if(newnode == NULL)
    {
        perror("malloc fail");
        return NULL;
    }
    newnode->next=NULL;
    newnode->data=x;
    return newnode;
}

//尾删
void SLPopBack(SLNode** pphead)
{
    assert(pphead);
    assert(*pphead);
    if((*pphead)->next==NULL)
    {
        free(*pphead);
        *pphead=NULL;
    }
    else 
    {
        SLNode* cur = *pphead;
        SLNode* prev = NULL;
        while(cur->next != NULL)
        {
            prev=cur;
            cur=cur->next;
        }
        //prev cur 
        free(cur);
        cur=NULL;
        prev->next = NULL;
    }
}

//头删
void SLPopFront(SLNode** pphead)
{
    assert(*pphead);
    assert(pphead);
    SLNode* front = *pphead;
    *pphead = front->next;
    free(front);
    front=NULL;
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
        cur = cur->next;
    }
    return NULL;
}

//插入
void SLInsertFront(SLNode** pphead,SLNode* pos,SLDataType x)
{
    assert(pos);
    assert(*pphead);
    assert(pphead);
    if(*pphead==pos)
    {
        SLPushFront(pphead,x);
    }
    else 
    {
        SLNode* newnode = SLBuyNode(x);
        SLNode* cur = *pphead;
        //cur pos
        while(cur->next!=pos)
        {
            cur=cur->next;
        }
        cur->next = newnode;
        newnode->next=pos;
    }
}

//删除
void SLErease(SLNode** pphead,SLNode* pos)
{
    assert(pos);
    assert(*pphead);
    assert(pphead);
    if(*pphead==pos)
    {
        SLPopFront(pphead);
    }
    else 
    {
        SLNode* cur = *pphead;
        while(cur->next!=pos)
        {
            cur=cur->next;
        }
        cur->next=pos->next;
        free(pos);
        pos=NULL;
    }
}
//删除
void SLEreaseAfter(SLNode** pphead,SLNode* pos)
{
    assert(pos);
    assert(*pphead);
    assert(pphead);
    //pos 
    SLNode* cur = pos->next;
    pos->next = cur->next;
    free(cur);
    cur=NULL;
}

void SLPrint(SLNode* phead)
{
    SLNode* cur = phead;
    while(cur)
    {
        printf("%d->",cur->data);
        cur=cur->next;
    }
    printf("NULL\n");
}
