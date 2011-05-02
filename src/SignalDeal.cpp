// Rocky 2009-08-26 18:09:36
#include "SignalDeal.h"
#include "Common.h"
#include <stdio.h>






int SignalDeal::m_signo = 0;


/*
 * ���ܣ�ȫ���źų�ʼ������װ�źŴ������ȣ�
 * ��д��Rocky 2009-08-26 18:20:47
 * ���أ���ȷ����>=0ֵ������<0ֵ��
 */
int SignalDeal::init()
{
    FUNCTION_TRACK(); // �����켣����

    struct sigaction act;

    // ��װ�źŴ�����
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = deal;
    if( sigaction(SIGTERM, &act, NULL) < 0
        || sigaction(SIGINT, &act, NULL) < 0
        || sigaction(SIG_EXIT, &act, NULL) < 0 )
    {
        return -2;
    }


    // // ����SIGTERM��SIGINT��SIG_EXIT���������������źţ�
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
 * ���ܣ����ؽ����Ƿ�ӵ��˳��ź�
 * ��д��Rocky 2009-08-26 18:20:47
 * ���أ����������˳�����1�������˳�����0��
 */
int SignalDeal::exit()
{
    // �ӵ�kill�����������������Զ�����˳��źţ�ʵʱ����
    return ( SIGTERM == m_signo
            || SIGINT == m_signo
            || SIG_EXIT == m_signo ) ? 1 : 0;
}

/*
 * ���ܣ��źŴ�����
 * ��д��Rocky 2009-08-26 18:29:25
 * ���أ����������˳�����>=0ֵ�������˳�����<0ֵ��
 */
void SignalDeal::deal(int signo, siginfo_t*, void*)
{
    m_signo = signo;

    // ע�⣬���źŴ�����ʹ��io�ǲ���ȫ�ġ�
    // Debug("�յ��źţ�[%d]", signo);
}
