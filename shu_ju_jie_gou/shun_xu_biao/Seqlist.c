#include"Seqlist.h"


//初始化顺序表
void SLInit(SL* ps)
{
    assert(ps);
    ps->a=(SLDataType*)malloc(sizeof(SLDataType)*Init_Capacity);
    if(ps->a==NULL)
    {
        perror("malloc fail");
        return;
    }
    ps->capacity=Init_Capacity;
    ps->size=0;
}

//销毁
void SLDestory(SL* ps)
{
    assert(ps);
    free(ps->a);
    ps->a=NULL;
    ps->capacity=0;
    ps->size=0;
}

//扩容
void SLCheckCapacity(SL* ps)
{
    assert(ps);
    SLDataType* temp = (SLDataType*)realloc(ps->a,sizeof(SLDataType)*(ps->capacity)*Check_Capacity);
    if(temp == NULL)
    {
        perror("realloc fail");
        return;
    }
    ps->a=temp;
    ps->capacity*=2;
}

//头删
void SLPopFront(SL* ps)
{
    assert(ps);
    assert(ps->size>=1);
    assert(ps->size >= 1);
    int begin = 0;
    while(begin<ps->size-1)
    {
        ps->a[begin]=ps->a[begin+1];
        begin++;
    }
    ps->size--;
}

//尾删
void SLPopBack(SL* ps)
{
    assert(ps);
    assert(ps->size>=1);
    ps->size--;
}

//头插
void SLPushFront(SL* ps, SLDataType x)
{
    assert(ps);
    if(ps->capacity==ps->size)
    {
        SLCheckCapacity(ps);
    }
    int end = ps->size - 1;
    while(end >= 0)
    {
        ps->a[end + 1] = ps->a[end];
        end--;
    }
    ps->a[0] = x;
    ps->size++;
}

//尾插
void SLPushBack(SL* ps,SLDataType x)
{
    assert(ps);
    if(ps->capacity==ps->size)
    {
        SLCheckCapacity(ps);
    }
    ps->a[ps->size] = x;
    ps->size++;
}

//pos位置插入
void SLInsertPos(SL* ps,int pos,SLDataType x)
{
    assert(ps);
    assert(pos>=0&&pos<=ps->size);
    if(ps->size==ps->capacity)
    {
        SLCheckCapacity(ps);
    }
    int end = ps->size - 1;
    while(end >= pos)
    {
        ps->a[end + 1] = ps->a[end];
        end--;
    }
    ps->a[pos] = x;
    ps->size++;
}

//pos位置删除
void SLErease(SL* ps,int pos)
{
    assert(ps);
    assert(pos>=0&&pos<ps->size);
    int begin = pos;
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
    int i = 0;
    for(i=0;i<ps->size;i++)
    {
        printf("%d ",ps->a[i]);
    }
    printf("\n");
}

//查找
int SLFind(SL* ps,SLDataType x)
{
    assert(ps);
    int i = 0;
    for(i=0;i<ps->size;i++)
    {
        if(ps->a[i]==x)
        {
            return i;
        }
    }
    return -1;
}
