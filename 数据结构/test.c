#include"Seqlist.h"
int main()
{
    SL ps;
    SLInit(&ps);
    SLPushFront(&ps,8);
    SLPushFront(&ps,7);
    SLPushFront(&ps,6);
    SLPushFront(&ps,5);
    SLPopFront(&ps);
    SLPrint(&ps);
    SLPushFront(&ps,4);
    SLPopBack(&ps);
    SLPrint(&ps);
    SLPushFront(&ps,3);
    SLPushFront(&ps,2);
    SLPopBack(&ps);
    SLPrint(&ps);
    SLPushFront(&ps,1);
    SLPrint(&ps);
    SLDestory(&ps);
}
