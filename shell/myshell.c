#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<assert.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#define LEFT "["
#define RIGHT "]"
#define LABLE "$"
#define LINE_SIZE 1024
#define ARGC_SIZE 32
#define DELIM " \t"
#define EXIT_CODE 44 //退出码

#define NONE        -1
#define IN_RDIR     0   //输入重定向
#define OUT_RDIR    1   //输出重定向
#define APPEND_RDIR 2   //追加重定向

int lastcode = 0;
int quit = 0;
extern char** environ;
char commandline[LINE_SIZE];
char* argv[ARGC_SIZE] = { NULL }; 
char pwd[LINE_SIZE];
char myenv[LINE_SIZE];
char *rdirfilename = NULL;
int rdir = NONE;
const char* getusername()
{
    //获取用户名
    return getenv("USER");
}
const char* gethostname()
{
    //获取主机名
    return getenv("HOSTNAME"); 
}
void getpwd()
{
    getcwd(pwd,sizeof(pwd));
}
void check_redir(char *cmd)
{
    //ls -al -n
    //ls -al -n >/</>> filename.txt
    char *pos = cmd;
    while(*pos)
    {
        if(*pos == '>')
        {
            if(*(pos + 1) == '>')
            {
                *pos++ = '\0';
                *pos++ = '\0';
                while(isspace(*pos))
                {
                    pos++;
                }
                rdirfilename = pos;
                rdir = APPEND_RDIR;
                break;
            }
            else 
            {
                *pos = '\0';
                pos++;
                while(isspace(*pos))
                {
                    pos++;
                }
                rdirfilename = pos;
                rdir = OUT_RDIR;
                break;
            }
        }    
        else if(*pos == '<')
        {
            *pos = '\0';
            pos++;
            while(isspace(*pos))
            {
                pos++;
            }
            rdirfilename = pos;
            rdir=IN_RDIR;
            break;
        }
        else 
        {
            
        }
        pos++;//遍历字符串
    }
}
void interact(char* cline,int size)
{  
    getpwd();
    printf(LEFT"%s@%s %s"RIGHT""LABLE" ",getusername(),gethostname(),pwd);
    char* s = fgets(cline,size,stdin);//只要输入必有内容
    assert(s!=NULL);//编译的时候有效果,运行的时候没有效果,debug下有效果
    (void)s;//s在后面不使用,防止编译器报错或者警告
    cline[strlen(cline)-1]='\0';//fgets读取到了回车的\n,去掉\n
    //ls -a -l > myfile.txt
    check_redir(cline);
}
int splitstring(char cline[],char* _argv[])
{
    int i = 0; 
    argv[i++] = strtok(cline,DELIM);
    while(_argv[i++] = strtok(NULL,DELIM));
    return i-1;
}
void NormalExcute(char* _argv[])
{
    pid_t id = fork();
    if(id < 0)
    {
        perror("fork");
        return;
    }
    else if(id == 0)
    {
        int fd = 0;
        //进程历史打开的文件与进行的各种重定向关系都和未来进行程序替换无关
        //程序替换并不影响文件访问
        if(rdir == IN_RDIR)
        {
            fd = open(rdirfilename,O_RDONLY);
            dup2(fd,0);
        }
        else if(rdir == OUT_RDIR)
        {
            fd = open(rdirfilename,O_CREAT|O_WRONLY|O_TRUNC,0666);
            dup2(fd,1);
        }
        else if(rdir == APPEND_RDIR)
        {
            fd = open(rdirfilename,O_CREAT|O_WRONLY|O_APPEND,0666);
            dup2(fd,1);
        }
        //子进程执行命令
        //execvpe(_argv[0],_argv,environ);
        execvp(_argv[0],_argv);
        exit(EXIT_CODE);//退出码
    }
    else 
    {
        int status = 0;
        pid_t rid = waitpid(id,&status,0);
        if(rid==id)
        {
            lastcode = WEXITSTATUS(status);
        }
    }
}
int buildCommand(char*_argv[],int _argc)
{
    if(_argc == 2 && strcmp(_argv[0],"cd")==0)
    {
        chdir(_argv[1]);
        getpwd();
        sprintf(getenv("PWD"),"%s",pwd);
        //putenv(argv[1]);
        return 1;
    }
    else if(_argc == 2 && strcmp(_argv[0],"export")==0)
    {
        strcpy(myenv,_argv[1]);
        putenv(myenv);
        return 1;
    }
    else if(_argc == 2 && strcmp(_argv[0],"echo") == 0)
    {
        if(strcmp(_argv[1],"$?")==0)
        {
            printf("%d\n",lastcode);
            lastcode = 0;
        }
        if(*_argv[1] == '$')
        {
            char* val = getenv(_argv[1]+1);
            if(val)
            {
                printf("%s\n",getenv(val));
            }
        }
        else 
        {
            printf("%s\n",_argv[1]);
        }
        return 1;
    }
    //特殊处理ls
    if(strcmp(_argv[0],"ls")==0)
    {
        _argv[_argc++] = "--color";
        _argv[_argc] = NULL;
    }
    return 0;
}
int main()
{
    while(!quit)
    {
        rdirfilename = NULL;
        rdir = NONE;
        //交互问题,获取命令行
        interact(commandline,sizeof(commandline));

        //子串分割的问题,解析命令行
        int argc = splitstring(commandline,argv);
        if(argc == 0)
        {
            continue;
        } 
        //指令的判断
        //内建命令本质上是shell内部的一个函数
        int n = buildCommand(argv,argc);

        // ls -a -l | wc -l
        // 分析输入的命令行字符串,获取有多少个|,命令打散多个子命令字符串,
        // malloc申请空间,pipe先申请多个管道,
        // 循环创建多个子进程,每一个子进程的重定向情况
        // 最开始,输出重定向,1->指定的一个管道的写端,
        // 中间,输入输出重定向,0标准输入重定向到上一个管道的读端,1标准输出重定向到下一个管道的写端
        // 最后一个,输入重定向,将标准输入重定向到最后一个管道的读端
        // 分别让不同的子进程执行不同的命令 --- exec* --- exec*不会影响该进程曾经打开的文件,不会影响预先设置好的管道重定向 
        // 普通命令的执行
        if(!n)
        {
            NormalExcute(argv);
        }
    }
    return 0;
}
