#include "ProcessPool.hpp"

const int processnum = 10;
vector<task_t> tasks;
//先描述
class channel
{
public:
    channel(int cmdfd,int slaverid,const string& processname)
        :_cmdfd(cmdfd)
        ,_slaverfd(slaverid)
        ,_processname(processname)
    {}
public:
    int _cmdfd;          //发送任务的文件描述符
    pid_t _slaverfd;     //子进程的PID
    string _processname; //子进程的名字 --- 方便打印日志
};
void slaver()
{
    //read(0)
    while(true)
    {
        int cmdcode = 0;
        int n = read(0,&cmdcode,sizeof(int));//如果父进程不给子进程发送数据呢？阻塞等待
        if(n == sizeof(int))
        {
            //执行的cmdcode对应的任务列表
            cout << "slaver say@ get a command" << getpid() << " : cmdode:" << cmdcode << endl;
            if(cmdcode >= 0 && cmdcode < tasks.size())
            {
                tasks[cmdcode]();
            }
        }
        if(n == 0)
        {
            break;
        }
    }
}


// void slaver(int rfd)
// {
//     while(true)
//     {
//         cout << getpid() << " - " << "read fd is:" << rfd << endl;
//         sleep(1);
//     }
// }


// 输入:const &
// 输出:*
// 输入输出:&
void InitProcessPool(vector<channel> *channels)
{
    //debug2:确保每一个子进程都有一个写端
    vector<int> oldfds;
    for(int i = 0;i < processnum;i++)
    {
        int pipefd[2];     //临时空间
        int n = pipe(pipefd);
        assert(!n);        //演示就可以
        (void)n;
        pid_t id = fork();
        if(id == 0)        //child
        {
            //子进程继承父进程的文件描述符和数组
            //只保留一个端口
            cout << "child: " << getpid() << " ,close history fd: " ;
            for(auto fd : oldfds)
            {
                cout << fd << " ";
                close(fd);
            }
            cout << "\n";
            close(pipefd[1]); //关闭写端
            dup2(pipefd[0],0);//从键盘改为从管道
            close(pipefd[0]); //关闭还是不关闭都可以
            slaver();
            cout << "process :" << getpid() << "quit" << endl;
            // slaver(pipefd[0]);
            exit(0);
        }
        //father
        close(pipefd[0]);//关闭读端
        //添加channel
        string name = "process-" + to_string(i);
        channels->push_back(channel((pipefd[1]),id,name));
        //保存上一个父进程的端口
        oldfds.push_back(pipefd[1]);
        sleep(1);
    }
}
void DeBug(const vector<channel>& channels)
{
    for(const auto &c:channels)
    {
        cout << c._cmdfd << " " << c._processname << " " << c._slaverfd << endl;
    }
}
void Menu()
{
    cout << "#######################################################################" << endl;
    cout << "#####   1. 刷新日志            2. 刷新出来野怪       ####################" << endl;
    cout << "#####   3. 检测软件是否更新     4. 更新血量或者蓝量   ####################" << endl;
    cout << "#####   0. 退出                                    #####################" << endl;
    cout << "########################################################################" << endl;
}
void CtrlSlaver(vector<channel>& channels)
{
    int which = 0;
    // int cnt = 5;
    while(true)
    {
        int slect = 0;
        Menu();
        cout<<"Please Enter@ ";
        std::cin >> slect;
        if(slect <= 0 || slect >= 5)
        {
            break;
        }       
        //1. 选择任务
        // int cmdcode = rand()%tasks.size();
        int cmdcode = slect - 1;
        //2. 选择进程
        //负载均衡
        //随机数
        // int processpos = rand()%channels.size();
        // cout << "father say: " << "cmdcode: " << cmdcode << "already sendto " << channels[processpos]._slaverfd << "process name: " << channels[processpos]._processname << endl;
        
        //轮询
        cout << "father say: " << "cmdcode: " << cmdcode << "already sendto " 
            << channels[which]._slaverfd << "process name: " 
                << channels[which]._processname << endl;
        //3. 发送任务
        write(channels[which]._cmdfd,&cmdcode,sizeof(cmdcode));
        which++;
        which%=channels.size();//防止越界
        // cnt--;
        sleep(1);
    }
}
void QuitProcess(const vector<channel>& channels)
{
    //debug 1
    //倒着回收
    // int last = channels.size() - 1;
    // for(int i = last; i >= 0;i--)
    // {
    //     close(channels[i]._cmdfd);
    //     waitpid(channels[i]._slaverfd,nullptr,0);
    // }

    for(const auto &c:channels)
    {
        close(c._cmdfd);
        waitpid(c._slaverfd,nullptr,0);
    }
    sleep(5);
    for(const auto &c:channels)
    {
        waitpid(c._slaverfd,nullptr,0);
    }
    sleep(5);
}
//再组织
int main()
{
    LoadTask(&tasks);
    srand(time(nullptr)^getpid()^1023);//种一个随机数种子,让时间更离散
    vector<channel> channels;
    // 1. 初始化
    InitProcessPool(&channels);
    //test
    DeBug(channels);
    // 2. 开始控制子进程
    CtrlSlaver(channels);
    
    // 3. 清理收尾
    QuitProcess(channels);
    return 0;
}