#include"process2.h"
//rate:[0, 100]
const char* lable = "|/-\\";
char bar[NUM];
void process2(int rate)
{ 
    if(rate < 0 || rate > 100)
    {
        return;
    }
    int len = strlen(lable);
    printf(BLUE"[%-100s]"END"[%d%%][%c]\r",bar,rate,lable[rate%len]);
    fflush(stdout);
    bar[rate++] = BODY;
    if(rate < 100)
    {
        bar[rate] = RIGHT;
    }
}

void Init()
{
    memset(bar,'\0',sizeof(bar));
}
