#include"SList.h"
int main()
{
    SLNode* phead = NULL;
    SLPushBack(&phead,8);
    SLPushBack(&phead,7);
    SLPopBack(&phead);
    SLPushBack(&phead,6);
    SLPushBack(&phead,5);
    SLPushBack(&phead,4);
    SLPushBack(&phead,3);
    SLPopFront(&phead);
    SLPushBack(&phead,2);
    SLPopBack(&phead);
    SLPushBack(&phead,1);
    SLPrint(phead);
    SLDestory1(phead);
    return 0;
}
