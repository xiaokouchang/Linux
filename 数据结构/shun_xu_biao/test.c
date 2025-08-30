#include"Seqlist.h"
int main()
{
    SL ps;
    SLInit(&ps);
    SLPushFront(&ps,4);
    SLPushFront(&ps,3);
    SLPushFront(&ps,2);
    SLPushFront(&ps,1);
    SLPopBack(&ps);
    SLPushBack(&ps,5);
    SLPushBack(&ps,6);
    SLErease(&ps,2);
    SLPushBack(&ps,7);
    SLPushBack(&ps,8);
    SLPopFront(&ps);
    SLPushBack(&ps,9);
    SLInsert(&ps,3,15);
    printf("%d\n",SLFind(&ps,7));
    SLPrint(&ps);
    SLDestory(&ps);
    return 0;
}
