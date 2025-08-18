#include"Seqlist.h"

void SLInit(SL* ps)
{
    ps->a=(DataType*)malloc(sizeof(DataType)*Init_Capacity);
    if(ps->a==NULL)
    {
        perror("malloc fail");
        return;
    }
    ps->capacity = Init_Capacity;
    ps->size = 0;
}
void SLDestory(SL* ps)
{
    assert(ps);
    free(ps->a);
    ps->a = NULL;
    ps->capacity = 0;
    ps->size = 0;
}

void SLPushBack(SL* ps,DataType x)
{
    assert(ps);
    if(ps->capacity == ps->size)
    {
        SLCheckCapacity(ps);
    }
    ps->a[ps->size] = x; 
    ps->size++;
}

void SLCheckCapacity(SL* ps)
{
    DataType* tmp = (DataType*)realloc(ps->a,sizeof(DataType)*2*ps->capacity);
    if(tmp == NULL)
    {
        perror("realloc fail");
        return;
    }
    ps->a=tmp;
    ps->capacity *= 2;
}

void SLPopBack(SL* ps)
{
    assert(ps);
    assert(ps->size>=0);
    ps->size--;
}


void SLPushFront(SL* ps,DataType x)
{
    assert(ps);
    if(ps->size==ps->capacity)
    {
        SLCheckCapacity(ps);
    }
    int end = ps->size-1;
    while(end >= 0)
    {
        ps->a[end + 1]=ps->a[end];
        end--;
    }
    ps->a[0] = x;
    ps->size++;
}

void SLPopFront(SL* ps)
{ 
    assert(ps);
    assert(ps->size>=0);
    int begin=0;
    while(begin<ps->size-1)
    {
        ps->a[begin]=ps->a[begin+1];
        begin++;
    }
    ps->size--;
}


void SLPrint(SL* ps)
{
    assert(ps);
    for(size_t i = 0;i<ps->size;i++)
    {
        printf("%d ",ps->a[i]);
    }
    printf("\n");
}


