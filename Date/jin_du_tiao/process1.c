#include"process1.h"
const char* lable = "|/-\\";
void process1(int speed)
{
  char bar[NUM];
  memset(bar,'\0',sizeof(bar));
  int cnt = 0;
  int len = strlen(lable); 
  while(cnt <= TOP)
  {
      printf("[%-100s][%d%%][%c]\r",bar,cnt,lable[cnt%len]);//没有\n,就没有立即刷新,因为显示器模式是行刷新
      fflush(stdout);
      bar[cnt++] = BODY;
      if(cnt < TOP)
      {
          bar[cnt] = RIGHT;
      }
      usleep(speed);
  }
  printf("\n");
}
