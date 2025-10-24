#include "Log.hpp"

int main()
{
    Log lg;
    lg.Enable(Onefile); // 启用单文件日志模式
    lg(Info, "test info log");
    lg(Error, "test error log: errno=%d", 2);
    lg(Warning, "test warning log");
    return 0;
}