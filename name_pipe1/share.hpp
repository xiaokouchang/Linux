#pragma once
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

#define FIFO_FILE "./fifo"
#define MODE 0664
enum
{
    FIFO_CREAT_ERR = 1,
    FIFO_DELETE_ERR,
    FIFO_OPEN_ERR,
};

class Init
{
public:
    Init()
    {
        //创建管道
        int n = mkfifo(FIFO_FILE, MODE);
        if(n == -1)
        {
            //创建失败
            perror("mkfifo");
            exit(FIFO_CREAT_ERR);
        }
    }
    ~Init()
    {
        int m = unlink(FIFO_FILE);
        if(m == -1)
        {
            perror("unlink");
            exit(FIFO_DELETE_ERR);
        }
    }
};