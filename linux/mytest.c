#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#define N 5
#define TASK_NUM 10
void task1()
{
    printf("这是一个执行打印日志的任务,pid:%d\n",getpid());
}
void task2()
{
    printf("这是一个执行检测网络健康状态的一个任务,pid:%d\n",getpid());
}
void task3()
{
    printf("这是一个绘制图形界面的任务,pid:%d\n",getpid());
}
typedef void(*task_t)();
task_t tasks[TASK_NUM];
int AddTask(task_t t);
void Init_Task()
{
    for(int i = 0;i<TASK_NUM;i++)
    {
        tasks[i]=NULL;
    }
    AddTask(task1);
    AddTask(task2);
    AddTask(task3);
}
int AddTask(task_t t)
{
    int pos = 0;
    for(pos = 0;pos<TASK_NUM;pos++)
    {
        if(tasks[pos] == NULL)
        {
            break;
        }
    }
    if(pos == TASK_NUM)
    {
        return -1;
    }
    tasks[pos] = t;
    return 0;
}
void ExecuteTask()
{
    for(int i = 0;i<TASK_NUM;i++)
    {
        if(!tasks[i])
        {
            continue;
        }
        tasks[i]();
    }
}
int main()
{
    pid_t id = fork();
    if(id<0)
    {
        perror("fork");
        return 1;
    }
    else if(id == 0) 
    {
        //子进程
        int cnt = 5;
        while(cnt)
        {
            printf("I am child,pid:%d,ppid:%d,cnt:%d\n",getpid(),getppid(),cnt);
            cnt--;
            sleep(1);
        }
        exit(11);
    }
    else 
    {
        int status = 0;
        Init_Task();
        AddTask(task1);
        AddTask(task2);
        AddTask(task3);
        while(1)//轮询
        {
            pid_t ret = waitpid(id,&status,WNOHANG);//非阻塞
            if(ret > 0)
            {
                //0x7F 0111 1111
                //printf("wait success,ret:%d,exit sig:%d,exit code:%d\n",ret,status&0x7F,(status>>8)&0xFF);
                if(WIFEXITED(status))
                {
                    printf("process success,code exit:%d\n",WEXITSTATUS(status));
                }
                else 
                {
                    printf("process fail\n");
                }
                break;
            }
            else if(ret<0)
            {
                printf("wait fail\n");
                break;
            }
            else 
            {
                ExecuteTask();
            }
            sleep(1);
        }
    }    
    sleep(3);
    return 0;
}
