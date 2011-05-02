#ifndef __SIGNALDEAL_H_20090826__
#define __SIGNALDEAL_H_20090826__
#include <signal.h>


// �źŴ�����
class SignalDeal
{
public:
    enum {
        SIG_EXIT = 63,  // �˳��źţ�ֵ63
    };

public:
    // ��ʼ������װ�źŵȣ�
    static int init();

    // �����˳����
    static int exit();

private:
    // �źŴ�����
    static void deal(int signo, siginfo_t*, void*);

private:
    static int m_signo;
};




#endif // end of __SIGNALDEAL_H_20090826__
