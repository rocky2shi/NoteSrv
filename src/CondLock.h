#ifndef __CONDLOCK_H_20100425__
#define __CONDLOCK_H_20100425__
#include "Common.h"
namespace CONDLOCK_SPACE
{







// ������
class CondLock
{
public:
    //
    int Init();

    // �����ȴ���
    int Wait();

    // ���������ѵȴ����㲥��ʽ��
    int Wake();


private:
    pthread_cond_t m_cond;      // ������ֻ֧��linux��ʵ�֣��ȸġ�[XXX]
    pthread_mutex_t m_mutex;
};





}// end of CONDLOCK_SPACE
using namespace CONDLOCK_SPACE;
#endif // end of __CONDLOCK_H_20100425__
