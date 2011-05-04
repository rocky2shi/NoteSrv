#ifndef __SIGNALDEAL_H_20090826__
#define __SIGNALDEAL_H_20090826__
#include <signal.h>


// 信号处理类
class SignalDeal
{
public:
    enum {
        SIG_EXIT = 63,  // 退出信号，值63
    };

public:
    // 初始化，安装信号等；
    static int init();

    // 进程退出检测
    static int exit();

private:
    // 信号处理函数
    static void deal(int signo, siginfo_t*, void*);

private:
    static int m_signo;
};




#endif // end of __SIGNALDEAL_H_20090826__
