#include"UDPServer.hpp"
#include<memory>
void Usage(string proc)
{
    cout << "\n\tUsage: " << proc << " port[1024+]" << endl;
}
int main(int argc,char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }
    // unique_ptr<UDPServer> svr(new UDPServer());
    // unique_ptr<UDPServer> svr(new UDPServer(8080,"114.132.66.75"));
    // unique_ptr<UDPServer> svr(new UDPServer(8080));
    // unique_ptr<UDPServer> svr(new UDPServer(80));
    uint16_t port = std::stoi(argv[1]);
    unique_ptr<UDPServer> svr(new UDPServer(port));
    svr->Init();
    svr->Run();
    return 0;
}


