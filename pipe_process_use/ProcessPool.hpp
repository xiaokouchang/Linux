#pragma once 
#include<iostream>
#include<string>
#include<vector>
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>
#include<ctime>
#include<sys/wait.h>
#include<sys/stat.h>
using namespace std;
typedef void (*task_t)();

void task1()
{
    cout << "lol 刷新日志" << endl;
}

void task2()
{
    cout << "lol 更新野区,刷新出来野怪" << endl;
}

void task3()
{
    cout << "lol 检测软件是否更新,如果需要,就提示用户" << endl;
}

void task4()
{
    cout << "lol 用户释放技能,更新血量或者蓝量" << endl;
}

void LoadTask(vector<task_t> *tasks)
{
    tasks->push_back(task1);
    tasks->push_back(task2);
    tasks->push_back(task3);
    tasks->push_back(task4);
}
