# pragma once 
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#define BODY '-'
#define RIGHT '>'
#define NUM 102
#define BLUE "\033[1;94m"  //亮蓝色加粗
#define END "\033[m"       //重置颜色
extern void process2(int rate);
void Init();


