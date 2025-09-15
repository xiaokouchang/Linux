//防止文件被重复包含
//#pragma once 

#ifndef  __MYFILE_H__ 
#define  __MYFILE_H__ 
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<assert.h>
#include<unistd.h>
#define SIZE 1024
#define FLUSH_NOW 1
#define FLUSH_LINE 2
#define FLUSH_ALL  4
typedef struct IO_FILE
{
    int fileno; 
    int flag;
    //int in_pos;
    //char inbuffer[SIZE];
    char outbuffer[SIZE];
    int out_pos;
}_FILE;

_FILE* _fopen(const char* filename,const char* flag);

int _fwrite(_FILE* fp,const char* msg,int len);

void _fclose(_FILE* fp);

void _fflush(_FILE* fp);
#endif 
