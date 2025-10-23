#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
void Usage(string proc)
{
    cout << "\n\tUsage: " << proc << " serverip serverport\n"
         << endl;
}
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(0);
    }
    string serverip = argv[1];          // 服务器的IP地址
    uint16_t serverport = stoi(argv[2]); // 服务器的端口号
    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport); 
    server.sin_addr.s_addr = inet_addr(serverip.c_str());
    socklen_t len = sizeof(server);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        cout << "socker error" << endl;
        return 1;
    }
    // 客户端一定要绑定,客户端需要自己的IP和端口,只是不需要用户显示的绑定,由操作系统自由随机选择
    string message;
    char buffer[1024];
    while (true)
    {
        // 数据
        cout << "Please Enter@ ";
        getline(cin, message);

        // 给谁发
        sendto(sockfd, message.c_str(), message.size(), 0, (struct sockaddr *)&server, len);
        struct sockaddr_in temp;
        socklen_t len = sizeof(temp);
        ssize_t s = recvfrom(sockfd,buffer,1023,0,(struct sockaddr*)&temp,&len);
        if(s > 0)
        {
            buffer[s] = 0;
            cout << buffer << endl;
        }
    }
    close(sockfd);
    return 0;
}


