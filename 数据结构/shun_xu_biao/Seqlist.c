
#include"Seqlist.h"

void SLInit(SL* ps)
{
    assert(ps);
    ps->a = (SLDataType*)malloc(sizeof(SLDataType)* Init_Capacity);
    if(ps->a == NULL)
    {
        perror("malloc fail");
        return;
    }
    ps->size=0;
    ps->capacity = Init_Capacity;
}

//销毁
void SLDestory(SL* ps)
{
    assert(ps);
    free(ps->a);
    ps->a=NULL;
    ps->size=0;
    ps->capacity=0;
}

void SLCheckCapacity(SL* ps)
{
    SLDataType* tmp = (SLDataType*)realloc(ps->a,sizeof(SLDataType)*(ps->capacity)*2);
    if(tmp == NULL)
    {
        perror("realloc fail");
        return;
    }
    ps->a=tmp;
    ps->capacity*=2;
}
//尾插
void SLPushBack(SL* ps,SLDataType x)
{
    assert(ps);
    if(ps->size==ps->capacity)
    {
        SLCheckCapacity(ps);
    }
    ps->a[ps->size]=x;
    ps->size++;
}

//头插
void SLPushFront(SL*ps,SLDataType x)
{
    assert(ps);
    if(ps->size==ps->capacity)
    {
        SLCheckCapacity(ps);
    }
    int end = ps->size - 1;
    while(end >= 0)
    {
        ps->a[end+1] = ps->a[end];
        end--;
    }
    ps->a[0] = x;
    ps->size++;
}


//尾删
void SLPopBack(SL* ps)
{
    assert(ps);
    assert(ps->size>=0);
    ps->size--;
}

//头删
void SLPopFront(SL* ps)
{
    assert(ps);
    assert(ps->size>=0);
    int begin=0;
    while(begin < ps->size - 1)
    {
        ps->a[begin] = ps->a[begin+1];
        begin++;
    }
    ps->size--;
}

//输出
void SLPrint(SL* ps)
{
    assert(ps);
    for(size_t i = 0;i<ps->size;i++)
    {
        printf("%d ",ps->a[i]);
    }
    printf("\n");
}

//插入
void SLInsert(SL* ps,int pos,SLDataType x)
{
    assert(ps);
    assert(pos>=0&&pos<=ps->size);
    if(ps->size==ps->capacity)
    {
        SLCheckCapacity(ps);
    }
    //1 2 3 4
    //1 2 5 3 4
    int end = ps->size - 1;
    
    while(end >= pos)
    {
        ps->a[end+1] = ps->a[end];
        end--;
    }
    ps->a[pos]=x;
    ps->size++;
}
//删除
void SLErease(SL* ps,int pos)
{
    assert(ps);
    assert(pos>=0&&pos<ps->size);
    int begin = pos;
    while(begin<ps->size-1)
    {
        ps->a[begin]=ps->a[begin+1];
        begin++;
    }
    ps->size--;
}
int SLFind(SL* ps,SLDataType x)
{
    assert(ps);
    for(size_t i=0;i<ps->size;i++)
    {
        if(ps->a[i] == x)
        {
            return i+1;
        }
    }
    return 0;
}
