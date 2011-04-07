// Rocky 2009-08-26 18:09:36
#include "SignalDeal.h"
#include "Common.h"
#include <stdio.h>






int SignalDeal::m_signo = 0;


/*
 * 功能：全局信号初始化（安装信号处理函数等）
 * 编写：Rocky 2009-08-26 18:20:47
 * 返回：正确返回>=0值，出错返<0值；
 */
int SignalDeal::init()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    struct sigaction act;

    // 安装信号处理函数
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = deal;
    if( sigaction(SIGTERM, &act, NULL) < 0
        || sigaction(SIGINT, &act, NULL) < 0
        || sigaction(SIG_EXIT, &act, NULL) < 0 )
    {
        return -2;
    }


    // // 除了SIGTERM、SIGINT、SIG_EXIT，忽略其它所有信号；
    // int ret;
    // sigset_t mask;
    // sigfillset(&mask);
    // sigdelset(&mask, SIGTERM);
    // sigdelset(&mask, SIGINT);
    // sigdelset(&mask, SIG_EXIT);
    // ret = sigprocmask(SIG_BLOCK, &mask, NULL);
    // if(-1 == ret)
    // {
    //     return -1;
    // }

    return 0;
}


/*
 * 功能：返回进程是否接到退出信号
 * 编写：Rocky 2009-08-26 18:20:47
 * 返回：进退正在退出返回1，不需退出返回0；
 */
int SignalDeal::exit()
{
    // 接到kill命发来的依赖，或自定义的退出信号（实时）；
    return ( SIGTERM == m_signo
            || SIGINT == m_signo
            || SIG_EXIT == m_signo ) ? 1 : 0;
}

/*
 * 功能：信号处理函数
 * 编写：Rocky 2009-08-26 18:29:25
 * 返回：进退正在退出返回>=0值，不需退出返回<0值；
 */
void SignalDeal::deal(int signo, siginfo_t*, void*)
{
    m_signo = signo;

    // 注意，在信号处理里使用io是不安全的。
    // Debug("收到信号：[%d]", signo);
}
