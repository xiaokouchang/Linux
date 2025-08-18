#include"SList.h"
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
SLNode* SLBuyNode(SLDataType x)
{
    SLNode* newnode = (SLNode*)malloc(sizeof(SLNode));
    if(newnode == NULL)
    {
        perror("malloc fail");
        return NULL;
    }
    newnode->next = NULL;
    newnode->data=x;
    return newnode;
}

void SLPushBack(SLNode** pphead,SLDataType x)
{
    assert(pphead);
    SLNode* newnode = SLBuyNode(x);
    if(*pphead==NULL)
    {
        *pphead = newnode;
    }
    else
    {
        SLNode* tail = *pphead;
        while(tail->next!=NULL)
        {
            tail =tail->next;
        }
        tail->next=newnode;
    }
}

void SLPushFront(SLNode** pphead,SLDataType x)
{
    assert(pphead);
    SLNode* newnode = SLBuyNode(x);
    newnode->next = *pphead;
    *pphead=newnode;
}

void SLPopBack(SLNode** pphead)
{
    assert(pphead);
    assert(*pphead);
    if((*pphead)->next == NULL)
    {
        free(*pphead);
        *pphead = NULL;
    }
    else 
    {
        SLNode* tail = *pphead;
        SLNode* prev = NULL; 
        while(tail->next != NULL)
        {
            prev =tail;
            tail=tail->next;
        }
        free(tail);
        tail=NULL;
        prev->next=NULL;
    }
}

void SLPopFront(SLNode** pphead)
{
    assert(pphead);
    assert(*pphead);
    SLNode* front = (*pphead)->next;
    free(front);
    front = NULL;
    *pphead = front;
}

SLNode* SLFind(SLNode* phead,SLDataType x)
{
    SLNode* cur = phead;
    while(cur)
    {
        if(cur->data==x)
        {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

void SLInsertFront(SLNode** pphead,SLNode* pos,SLDataType x)
{
    assert(pos);
    assert(pphead);
    SLNode* newnode=SLBuyNode(x);
    if(*pphead==pos)
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
        //cur pos 
        cur->next =newnode;
        newnode->next = pos;
    }
}
void SLErease(SLNode** pphead,SLNode* pos)
{
    assert(pos);
    assert(pphead);
    assert(*pphead);
    if(*pphead == pos)
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
        cur->next = pos->next;
        free(pos);
        pos=NULL;
    }
}
void SLInsertAfter(SLNode**pphead,SLNode* pos,SLDataType x)
{
    assert(pos);
    assert(pphead);
    assert(*pphead);
    //pos 
    SLNode* newnode =SLBuyNode(x);
    SLNode* cur = pos->next;
    pos->next = newnode;
    newnode->next = cur;
}

void SLEreaseAfter(SLNode** pphead,SLNode* pos)
{
    assert(pos);
    assert(pphead);
    //pos 
    SLNode* cur = pos->next;
    pos->next = cur->next; 
    free(cur);
    cur=NULL;
}
