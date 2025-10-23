#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "log.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
const int size = 1024;
// 整数IP转化为字符串IP
// struct ip
// {
//     uint8_t part1;
//     uint8_t part2;
//     uint8_t part3;
//     uint8_t part4;
// };
// int src_ip = 123456789;//"123.456.789"
// struct ip *p = (struct ip*)src_ip;
// to_string(p->part1)+"."+p->to_string(p->part2)+"."+
//     to_string(p->part3)+"."+to_string(p->part4)

// "192.168.50.100" -> "192" "168" "50" "100"
// uint32_t IP;
// struct ip* x= (struct ip*)&ip;
// x->part1 = stoi("192");
// x->part2 = stoi("168");
// x->part3 = stoi("50");
// x->part4 = stoi("100");

uint16_t defaultport = 8080;
string defaultip = "0.0.0.0";
Log log;
enum
{
    SOCKET_ERR = 1, // 创建失败
    BIND_ERR,       // 绑定失败
};

class UDPServer
{
public:
    UDPServer(const uint16_t &port = defaultport, const string &ip = defaultip)
        : port_(port), ip_(ip), sockfd_(0), isrunning_(false)
    {
    }
    void Init()
    {
        // 1. 创建udp socket
        sockfd_ = socket(AF_INET, SOCK_DGRAM, 0); // AF_INET也可以写成PF_INET,二者是等价的
        if (sockfd_ < 0)
        {
            log(Fatal, "socket create error,sockfd: %d", sockfd_);
            exit(SOCKET_ERR);
        }
        log(Info, "socket create success,sockfd: %d", sockfd_);

        // 2. 绑定套接字
        // 在栈上填充了数据,local没有和套接字相关联
        struct sockaddr_in local; // 在地址空间的用户区的栈上
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(port_);                  // 需要保证端口号是网络字节序列1,因为该端口号是要给对方发送的
        // local.sin_addr.s_addr = inet_addr(ip_.c_str()); // string -> uint32_t,将整数IP转成字符串IP,uint32_t必须是网络序列的
        local.sin_addr.s_addr = htonl(INADDR_ANY); // 任意绑定
        int n = bind(sockfd_, (const struct sockaddr *)&local, sizeof(local));
        if (n < 0)
        {
            log(Fatal, "bind error,errno: %d,err string: %s", errno, strerror(errno));
            exit(BIND_ERR);
        }
        log(Info, "bind success: %d,err string: %s", errno, strerror(errno));
    }
    void Run()
    {
        isrunning_ = true;
        char inbuffer[size];
        while (isrunning_)
        {
            struct sockaddr_in client;
            socklen_t temp_len = sizeof(client);
            ssize_t n = recvfrom(sockfd_, inbuffer, sizeof(inbuffer) - 1, 0, (struct sockaddr *)&client, &temp_len);
            if (n < 0)
            {
                log(Warning, "revfrom error,errno: %d,err string: %s", errno, strerror(errno));
                continue;
            }
            inbuffer[n] = 0;
            log(Debug, "收到客户端[%s:%d]消息: %s",
                inet_ntoa(client.sin_addr), // 客户端IP
                ntohs(client.sin_port),     // 客户端端口（网络字节序转主机字节序）
                inbuffer);                  // 接收的消息内容
            string info = inbuffer;
            string echo_string = "server echo# " + info;
            sendto(sockfd_, echo_string.c_str(), echo_string.size(), 0, (const sockaddr *)&client, temp_len);
        }
    }
    ~UDPServer()
    {
        if (sockfd_ > 0)
        {
            close(sockfd_);
        }
    }

private:
    int sockfd_;    // 网络文件描述符
    uint16_t port_; // 表明服务器进程的端口号
    string ip_;     // 任意地址绑定
    bool isrunning_;
};



