#include "share.hpp"
//管理管道文件
int main()
{
    Init init;
    //打开管道
    //等待写入方打开之后,自己才会打开文件,向后执行,open会阻塞
    int fd = open(FIFO_FILE,O_RDONLY);
    if(fd < 0)
    {
        perror("open");
        exit(FIFO_OPEN_ERR);
    }
    cout << "server open file done" << endl;
    //开始通信
    while(true)
    {
        char buffer[1024] = {0};
        int x = read(fd,buffer,sizeof(buffer));
        if(x > 0)
        {
            buffer[x] = 0;
            cout << "client say# " << buffer << endl; 
        }
        else if(x == 0)
        {
            cout << "client quit,me too" << endl;
            break;
        }
        else 
        {
            break;
        }
    }
    
    //关闭管道
    close(fd);
    return 0;
}


// #include "share.hpp"
// #include "log.hpp"
// int main()
// {
//     logmessage(Info,"hello");
//     return 0;
// }


//添加日志
// #include "share.hpp"
// #include "log.hpp"
// //管理管道文件
// int main()
// {
//     Init init;
//     Log log;
//     // log.Enable(ONEFILE);
//     log.Enable(Classfile);
//     //打开管道
//     //等待写入方打开之后,自己才会打开文件,向后执行,open会阻塞
//     int fd = open(FIFO_FILE,O_RDONLY);
//     if(fd < 0)
//     {
//         log(Fatal,"error string: %s,error code:%d",strerror(errno),errno);
//         exit(FIFO_OPEN_ERR);
//     }
//     log(Info,"server open file done,error string: %s,error code:%d",strerror(errno),errno);
//     //开始通信
//     while(true)
//     {
//         char buffer[1024] = {0};
//         int x = read(fd,buffer,sizeof(buffer));
//         if(x > 0)
//         {
//             buffer[x] = 0;
//             cout << "client say# " << buffer << endl; 
//         }
//         else if(x == 0)
//         {
//             log(Debug,"client quit,me too,error string: %s,error code:%d",strerror(errno),errno);
//             break;
//         }
//         else 
//         {
//             break;
//         }
//     }
    
//     //关闭管道
//     close(fd);
//     return 0;
// }