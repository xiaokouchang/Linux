#include <iostream>
#include <cstdlib>  //stdlib.h
#include <unistd.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>

#define N 2 
#define NUM 1024 
using namespace std;
//child
void Writer(int wfd)
{
    string s = "hello,I am child";
    pid_t self = getpid();
    int number = 0;
    char buffer[NUM];//定义缓冲区
    while(true)
    {
        sleep(1);
        buffer[0] = 0;//字符串清空,将这个数组当做字符串
        snprintf(buffer,sizeof(buffer),"%s-%d-%d",s.c_str(),self,number);//将字符串连接并写入buffer
        number++;
        // cout << buffer << endl;
        // 发送/写入给父进程,系统调用
        write(wfd,buffer,strlen(buffer));//不需要加1
        // char c = 'c';
        // write(wfd,&c,strlen(buffer));//不需要加1
        // number++;
        // cout << number << endl;
        // if(number >= 5)
        // {
        //     break;
        // }
    }
}
//father
void Reader(int rfd)
{
    char buffer[NUM];
    int cnt = 0;
    while(true)
    {
        sleep(5);
        buffer[0] = 0;
        ssize_t n = read(rfd,buffer,sizeof(buffer)); 
        if(n > 0)
        {
            buffer[n] = 0; //0 == '\0',数字一样,类型不同
            cout << "father get a message[" << getpid() << "]# " << buffer << endl;
        }
        else if(n == 0)
        {
            printf("father read file done\n");
            break;
        }
        else 
        {
            break;
        }
        cnt++;
        if(cnt>5)
        {
            break;
        }
    }
}
int main()
{
    //创建管道
    int pipefd[N] = {0};
    int n = pipe(pipefd);
    if(n < 0)
    {
        return 1;
    }
    // cout << "pipefd[0]:" << pipefd[0] << ",pipefd[1]:" << pipefd[1] << endl;
    //创建子进程
    //child -> w, father -> r
    pid_t id = fork();
    if(id < 0)
    {
        return 2;
    }
    else if(id == 0)
    {
        //child
        close(pipefd[0]);//关闭读的一端
        Writer(pipefd[1]);
        close(pipefd[1]);//关闭写
        exit(0);
    }
    else 
    {
        close(pipefd[1]);//关闭写的一端
        Reader(pipefd[0]);
        close(pipefd[0]);
        cout << "father close read fd:" << pipefd[0] << endl;
        sleep(5); //为了观察僵尸
        int status = 0;
        pid_t rid = waitpid(id,&status,0);
        if(rid < 0)
        {
            return 3;
        }
        cout << "wait child success:" << rid << "exit code:" << ((status>>8)&0xFF) << "exit signal:" << (status&0x7F) << endl;
        sleep(5);
        cout << "father exit" << endl;
    }
    return 0;
}