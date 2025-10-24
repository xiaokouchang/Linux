#pragma once

#include <iostream>
#include <time.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
using namespace std;
#define SIZE 1024

#define Info 0
#define Debug 1
#define Warning 2
#define Error 3
#define Fatal 4

#define Screen 1
#define Onefile 2
#define Classfile 3

#define LogFile "log.txt"

class Log
{
public:
    Log()
    {
        printMethod = Screen;
        path = "./log/";
    }
    void Enable(int method)
    {
        printMethod = method;
    }
    std::string levelToString(int level)
    {
        switch (level)
        {
        case Info:
            return "Info";
        case Debug:
            return "Debug";
        case Warning:
            return "Warning";
        case Error:
            return "Error";
        case Fatal:
            return "Fatal";
        default:
            return "None";
        }
    }
    // void logmessage(int level, const char *format, ...)
    // {
    //     time_t t = time(nullptr);
    //     struct tm *ctime = localtime(&t);
    //     char leftbuffer[SIZE];
    //     snprintf(leftbuffer, sizeof(leftbuffer), "[%s][%d-%d-%d %d:%d:%d]", levelToString(level).c_str(),
    //              ctime->tm_year + 1900, ctime->tm_mon + 1, ctime->tm_mday,
    //              ctime->tm_hour, ctime->tm_min, ctime->tm_sec);

    //     // va_list s;
    //     // va_start(s, format);
    //     char rightbuffer[SIZE];
    //     vsnprintf(rightbuffer, sizeof(rightbuffer), format, s);
    //     // va_end(s);

    //     // 格式：默认部分+自定义部分
    //     char logtxt[SIZE * 2];
    //     snprintf(logtxt, sizeof(logtxt), "%s %s\n", leftbuffer, rightbuffer);

    //     // printf("%s", logtxt); // 暂时打印
    //     printLog(level, logtxt);
    // }
    void printLog(int level, const std::string &logtxt)
    {
        switch (printMethod)
        {
        case Screen:
            std::cout << logtxt << std::endl;
            break;
        case Onefile:
            printOneFile(LogFile, logtxt);
            break;
        case Classfile:
            printClassFile(level, logtxt);
            break;
        default:
            break;
        }
    }
    void printOneFile(const std::string &logname, const std::string &logtxt)
    {
        std::string _logname = path + logname;
        // 新增：打印日志路径，确认拼接是否正确
        std::cout << "[Debug] Log path: " << _logname << std::endl;

        int fd = open(_logname.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (fd < 0)
        {
            std::cerr << "open failed! errno: " << errno << ", msg: " << strerror(errno) << std::endl;
            return;
        }
        write(fd, logtxt.c_str(), logtxt.size());
        close(fd);
    }
    void printClassFile(int level, const std::string &logtxt)
    {
        std::string filename = LogFile;
        filename += ".";
        filename += levelToString(level); // "log.txt.Debug/Warning/Fatal"
        printOneFile(filename, logtxt);
    }

    ~Log()
    {
    }
    void operator()(int level, const char *format, ...)
    {
        time_t t = time(nullptr);
        struct tm *ctime = localtime(&t);
        char leftbuffer[SIZE];
        snprintf(leftbuffer, sizeof(leftbuffer), "[%s][%d-%d-%d %d:%d:%d]", levelToString(level).c_str(),
                 ctime->tm_year + 1900, ctime->tm_mon + 1, ctime->tm_mday,
                 ctime->tm_hour, ctime->tm_min, ctime->tm_sec);

        va_list s;
        va_start(s, format);
        char rightbuffer[SIZE];
        vsnprintf(rightbuffer, sizeof(rightbuffer), format, s);
        va_end(s);

        // 格式：默认部分+自定义部分
        char logtxt[SIZE * 2];
        snprintf(logtxt, sizeof(logtxt), "%s %s", leftbuffer, rightbuffer);

        // 新增：调试输出logtxt内容，确认是否有值
        printf("[Debug] logtxt content: %s\n", logtxt);
        // printf("%s", logtxt); // 暂时打印
        printLog(level, logtxt);
    }

private:
    int printMethod;
    std::string path;
};

// int sum(int n, ...)
// {
//     va_list s; // char*
//     va_start(s, n);

//     int sum = 0;
//     while(n)
//     {
//         sum += va_arg(s, int); // printf("hello %d, hello %s, hello %c, hello %d,", 1, "hello", 'c', 123);
//         n--;
//     }

//     va_end(s); //s = NULL
//     return sum;
// }