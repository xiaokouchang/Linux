#include"Seqlist.h"
int main()
{
    SL ps;
    SLInit(&ps);
    SLPushFront(&ps,9);
    SLPushFront(&ps,8);
    SLPushFront(&ps,7);
    SLPushFront(&ps,6);
    SLPushFront(&ps,5);
    SLPushFront(&ps,4);
    SLPushFront(&ps,3);
    SLErease(&ps,4);
    SLPushFront(&ps,2);
    SLPushFront(&ps,1);
    SLInsert(&ps,3,10);
    SLPrint(&ps);
    SLDestory(&ps);
    return 0;
}

