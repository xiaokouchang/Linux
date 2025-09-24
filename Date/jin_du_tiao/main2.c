#include"process2.h"
typedef void(*callback_t)(int);
//模拟安装或下载
void DownLoad(callback_t cb)
{
   int total = 1000;//容量
   int cur = 0;//进度
   while(cur <= total)
   {
       //进行着下载任务
       usleep(50000);//循环下载了一部分,更新进度
       int rate = cur * 100 / total;//更新进度
       cb(rate);//通过回调,展示进度
       cur += 10;//每次下载完成
   }
   printf("\n");
}
int main()
{
   printf("DownLoad 1: \n");
   DownLoad(process2);
   Init();
   printf("DownLoad 2: \n");
   DownLoad(process2);
   Init();
   printf("DownLoad 3: \n");
   DownLoad(process2);
   Init();
   printf("DownLoad 4: \n");
   DownLoad(process2);
   Init();
   return 0;
}

