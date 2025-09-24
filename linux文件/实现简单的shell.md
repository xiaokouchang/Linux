# 一、重 识 shell
## 1、定 义
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;shell 是 操 作 系 统 外 的 一 层 外 壳 程 序，负 责 帮 用 户 进 行 指 令 的 执 行。它 将 指 令 交 给 操 作 系 统，操 作 系 统 将 结 果 给 用 户。shell/bash 也 是 一 个 进 程，执 行 指 令 的 时 候，本 质 就 是 自 己 创 建 子 进 程 执 行 的。
## 2、实 现 简 单 的 shell
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;命 令 行 的 本 质 就 是 字 符 串。
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;当 我 们 进 行 登 陆 的 时 候，系 统 要 启 动 一 个 shell 进 程，shell 会 读 取 用 户 目 录 下 的 .bash_profile 文 件，里 面 保 存 了 导 入 环 境 变 量 的 方 式。
# 二、先 看 效 果：这 个 简 易 Shell 能 做 什 么？
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;在 分 析 代 码 前，先 明 确 目 标 功 能 - - - 这 段 代 码 实 现 了 一 个 具 备 基 础 能 力 的 Shell，支 持：
1. **命 令 行 交 互**：显 示 [用户名@主机名 路径]$ 格 式 的 提 示 符。
2. **部 分 内 建 命 令**：cd（切 换 目 录）、export（设 置 环 境 变 量）、echo（输 出 内 容，支 持 echo $? 和 echo $ENV）。
3. **普 通 命 令 执 行**：如 ls、pwd、date（通 过 fork + exec 实 现）。
4. **重 定 向**：支 持 >（覆 盖 输 出）、>>（追 加 输 出）、<（输 入 重 定 向）。
5. **ls 颜 色 优 化**：自 动 为 ls 命 令 添 加 --color 参 数，实 现 彩 色 输出。
![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/72c99a8602e64928bd6b745dc5dfdcc0.png)
***
# 三、环 境 准 备 与 编 译 运 行
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;由 于 代 码 依 赖 Linux 系 统 调 用（如 fork、waitpid、dup2），需 在 Linux 环 境 下 编 译 运 行：
## 1、依 赖 头 文 件
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;代 码 包 含 了 系 统 编 程（unistd.h、sys/types.h）、标 准 库（stdio.h、string.h）等 头 文 件，无 需 额 外 安 装 库。
***
## 2、编 译 命 令
**方 式 1**
```javascript
gcc myshell.c -o myshell   # 编译代码
./myshell                  # 运行自定义Shell
```
**方 式 2**

使 用 [make 和 makefile](https://blog.csdn.net/2301_78847073/article/details/150147085?spm=1001.2014.3001.5501)，makefile 的 代 码 见 文 章 结 尾。
```javascript
make       # 编译代码
./myshell  # 运行自定义shell
make clean # 清理可执行程序
```
***
## 3、退 出 方 式
可 以 按 Ctrl + C 退 出。
***
# 四、核 心 模 块 拆 解：从 交 互 到 执 行 的 全 流 程
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;代 码 按 “功 能 模 块 化” 设 计，每 个 函 数 对 应 一 个 核 心 步 骤。我 们 从 “用 户 输 入 → 命 令 执 行” 的 流 程 逐 步 解 析。
## 1、命 令 行 交 互
**功 能**：显 示 提 示 符、读 取 用 户 输 入、预 处 理 命 令 行（去 掉 换 行 符、触 发 重 定 向 检 查）。
```javascript
void interact(char* cline,int size)
{  
    getpwd();  // 获取当前路径（调用getcwd实现）
    // 显示提示符：[用户名@主机名 路径]$
    printf(LEFT"%s@%s %s"RIGHT""LABLE" ",getusername(),gethostname(),pwd);
    
    // 读取用户输入（fgets会包含末尾的换行符'\n'）
    char* s = fgets(cline,size,stdin);
    assert(s!=NULL);  // Debug阶段确保输入有效（Release模式失效）
    (void)s;  // 避免“未使用变量”警告
    
    cline[strlen(cline)-1]='\0';  // 去掉fgets读取的换行符'\n'
    check_redir(cline);  // 检查并处理重定向（如“ls > file.txt”）
}
```
**关 键 细 节**：
1. **提 示 符 格 式**：通 过 宏 定 义 LEFT="["、RIGHT="]"、LABLE="$" 统 一 控 制，便 于 修 改。
2. **路 径 / 用 户 名 / 主 机 名**：通 过 getcwd（当 前 路 径）、getenv("USER")（用 户 名）、getenv("HOSTNAME")（主 机 名）获 取；
3. **输 入 处 理**：fgets 读 取 命 令 行 后，必 须 去 掉 末 尾 的 \n（否 则 命 令 会 包 含 换 行 符，导 致 执 行 失 败）。
***
## 2、重 定 向 处 理
### （1）核 心 原 理
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;重 定 向 的 本 质 是 修 改 标 准 输 入 和 标 准 输 出 的 文 件 描 述 符 指 向。例 如 ls > file.txt 是 将 标 准 输 出（默 认 指 向 终 端）改 为 指 向 file.txt。
***
### （2）代 码 逻 辑
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;遍 历 命 令 行 字 符 串，识 别 >、>>、<，将 “命 令 部 分” 与 “文 件 名 部 分” 分 割，并 设 置 重 定 向 类 型 和 文 件 名。
```javascript
void check_redir(char *cmd)
{
    char *pos = cmd;
    while(*pos)
    {
        if(*pos == '>')
        {
            if(*(pos + 1) == '>')  // 识别“>>”（追加重定向）
            {
                *pos++ = '\0';  // 截断命令部分（如“ls>>file”→“ls”）
                *pos++ = '\0';
                while(isspace(*pos)) pos++;  // 跳过空格（如“ls >> file”）
                rdirfilename = pos;  // 记录文件名
                rdir = APPEND_RDIR;  // 设置重定向类型
                break;
            }
            else  // 识别“>”（覆盖重定向）
            {
                *pos = '\0';
                pos++;
                while(isspace(*pos)) pos++;
                rdirfilename = pos;
                rdir = OUT_RDIR;
                break;
            }
        }    
        else if(*pos == '<')  // 识别“<”（输入重定向）
        {
            *pos = '\0';
            pos++;
            while(isspace(*pos)) pos++;
            rdirfilename = pos;
            rdir=IN_RDIR;
            break;
        }
        pos++;  // 遍历字符串
    }
}
```
***
## 3、命 令 解 析
### （1）功 能
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;将 预 处 理 后 的 命 令 部 分 按 空 格 / 制 表 符 分 割，生 成  argv 数 组（如 ls -al → argv[0]="ls"、argv[1]="-al"、argv[2]=NULL）。
```javascript
int splitstring(char cline[],char* _argv[])
{
    int i = 0; 
    _argv[i++] = strtok(cline,DELIM);  // 第一次调用：传入原始字符串
    while(_argv[i++] = strtok(NULL,DELIM));  // 后续调用：传NULL，继续分割
    return i-1;  // 返回参数个数argc
}
```
***
### （2）关 键 函 数
1. **作 用**：按 指 定 分 隔 符（DELIM=" \t"，即 空 格 和 制 表 符）分 割 字 符 串。
2. **特 性**：第 一 次 调 用 需 传 入 原 始 字 符 串，后 续 调 用 传 NULL，会 自 动 从 上 次 分 割 的 位 置 继 续。
3. **终 止 条 件**：当 分 割 到 末 尾 时，strtok 返 回 NULL，循 环 终 止，argv 最 后 一 个 元 素 为 NULL（符 合 exec 函 数 的 参 数 要 求）。
***
## 4、内 建 命 令 处 理
### （1）为 什 么 需 要 内 建 命 令？
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;像 cd、export 这 类 命 令，不 能 通 过 fork + exec 执 行 - - - 因 为 fork 会 创 建 子 进 程，子 进 程 的 目 录 / 环 境 变 量 修 改 不 会 影 响 父 进 程 （Shell 本 身）。因 此，内 建 命 令 必 须 在 Shell 进 程 内 直 接 执 行。
***
### （2）cd：切 换 目 录
```javascript
if(_argc == 2 && strcmp(_argv[0],"cd")==0)
{
    chdir(_argv[1]);  // 切换目录（系统调用）
    getpwd();  // 更新当前路径缓存
    sprintf(getenv("PWD"),"%s",pwd);  // 更新PWD环境变量
    return 1;  // 标记为内建命令，无需后续执行
}
```
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;chdir 仅 修 改 当 前 进 程 的 目 录，需 手 动 更 新 PWD 环 境 变 量（否 则  echo $PWD 会 显 示 旧 路 径）。
***
### （3）export：设 置 环 境 变 量
```javascript
else if(_argc == 2 && strcmp(_argv[0],"export")==0)
{
    strcpy(myenv,_argv[1]);  // 将“KEY=VALUE”复制到全局变量myenv
    putenv(myenv);  // 加入环境变量表（系统调用）
    return 1;
}
```
**注 意**：putenv 要 求 参 数 指 向 的 字 符 串 在 后 续 不 被 修 改（否 则 环 境 变 量 会 失 效），因 此 用 全 局 变 量 myenv 存 储，避 免 栈 内 存 释 放 问 题。
***
### （4）echo：输 出 内 容
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;支 持 3 种 场 景：echo 字 符 串、echo $?（显 示 上 一 命 令 退 出 码）、echo $ENV（显 示 环 境 变 量 值）
```javascript
else if(_argc == 2 && strcmp(_argv[0],"echo") == 0)
{
    if(strcmp(_argv[1],"$?")==0)
    {
        printf("%d\n",lastcode);  // 显示上一命令退出码
        lastcode = 0;  // 重置退出码
    }
    else if(*_argv[1] == '$')
    {
        // 处理“echo $ENV”（如echo $HOME）
        char* val = getenv(_argv[1]+1);  // _argv[1]+1：跳过“$”
        if(val) printf("%s\n", val);  
    }
    else 
    {
        printf("%s\n",_argv[1]);  // 普通字符串输出
    }
    return 1;
}
```
***
### （5）ls 特 殊 优 化
为 ls 命 令 自 动 添 加 `--color` 参 数，实 现 彩 色 输 出。
```javascript
if(strcmp(_argv[0],"ls")==0)
{
    _argv[_argc++] = "--color";  // 追加--color参数
    _argv[_argc] = NULL;  // 确保argv末尾为NULL
}
```
***
## 5、普 通 命 令 执 行
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;对 于 ls、pwd、date 等 外 部 命 令，需 通 过 fork 创 建 子 进 程 → 处 理 重 定 向 → exec 替 换 程 序 的 流 程 执 行。
```javascript
void NormalExcute(char* _argv[])
{
    pid_t id = fork();  // 1. 创建子进程
    if(id < 0) { perror("fork"); return; }
    else if(id == 0)  // 2. 子进程：处理重定向 + 执行命令
    {
        int fd = 0;
        // 根据重定向类型修改文件描述符
        if(rdir == IN_RDIR)  // < 输入重定向（标准输入→文件）
        {
            fd = open(rdirfilename,O_RDONLY);
            dup2(fd,0);  // 将fd复制到0（标准输入）
        }
        else if(rdir == OUT_RDIR)  // > 覆盖输出（标准输出→文件）
        {
            fd = open(rdirfilename,O_CREAT|O_WRONLY|O_TRUNC,0666);
            dup2(fd,1);  // 将fd复制到1（标准输出）
        }
        else if(rdir == APPEND_RDIR)  // >> 追加输出
        {
            fd = open(rdirfilename,O_CREAT|O_WRONLY|O_APPEND,0666);
            dup2(fd,1);
        }

        // 执行命令（替换子进程代码）
        execvp(_argv[0],_argv);
        exit(EXIT_CODE);  // 若exec失败（如命令不存在），退出子进程
    }
    else  // 3. 父进程：等待子进程结束，记录退出码
    {
        int status = 0;
        pid_t rid = waitpid(id,&status,0);  // 等待指定子进程
        if(rid==id)
        {
            lastcode = WEXITSTATUS(status);  // 提取子进程退出码
        }
    }
}
```
***
### 关 键 原 理 解 析
（1）**fork 与 exec 的 配 合**
1. fork 创 建 子 进 程 后，子 进 程 的 代 码 与 父 进 程 完 全 一 致。
2. execvp 会 将 子 进 程 的 代 码 替 换 为 目 标 命 令（如 ls）的 代 码，原 有 的 Shell 代 码 不 再 执 行。
3. 若 execvp 执 行 失 败（如 命 令 不 存 在），子 进 程 会 执 行 exit(EXIT_CODE)，父 进 程 通 过 waitpid 获 取 退 出 码。

（2）**重 定 向 实 现（dup2）**
**dup2(fd, target)**：将 文 件 描 述 符 fd 复 制 到 target，覆 盖 target 原 有 的 指 向。

（3）**文 件 权 限**
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;open 时 指 定 0666（读 写 权 限），实 际 权 限 会 受 umask 影 响（通 常 为 0644）。
***
# 五、完 整 代 码 展 示
## 1、makefile
```javascript
myshell:myshell.c
	gcc -o $@ $^ -std=c99
.PHONY:clean
clean:
	rm -rf myshell 
```
## 2、myshell.c
```javascript
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
                printf("%s\n",val);
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
        //普通命令的执行
        if(!n)
        {
            NormalExcute(argv);
        }
    }
    return 0;
}
```