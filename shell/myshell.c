#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#define LEFT "["
#define RIGHT "]"
#define LABLE "$"
#define LINE_SIZE 1024
#define ARGC_SIZE 32
#define DELIM " \t"
#define EXIT_CODE 44 //退出码
int lastcode = 0;
int quit = 0;
extern char** environ;
char commandline[LINE_SIZE];
char* argv[ARGC_SIZE] = { NULL }; 
char pwd[LINE_SIZE];
char myenv[LINE_SIZE];
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
void interact(char* cline,int size)
{  
    getpwd();
    printf(LEFT"%s@%s %s"RIGHT""LABLE" ",getusername(),gethostname(),pwd);
    char* s = fgets(cline,size,stdin);//只要输入必有内容
    assert(s!=NULL);//编译的时候有效果,运行的时候没有效果,debug下有效果
    (void)s;//s在后面不使用,防止编译器报错或者警告
    cline[strlen(cline)-1]='\0';//fgets读取到了回车的\n,去掉\n
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
        //普通命令的执行
        if(!n)
        {
            NormalExcute(argv);
        }
    }
    return 0;
}
