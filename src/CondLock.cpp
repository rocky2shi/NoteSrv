// Rocky 2010-04-25 15:18:39
#include "CondLock.h"
#include <error.h>
namespace CONDLOCK_SPACE
{




int CondLock::Init()
{
    FUNCTION_TRACK(); // �����켣����

    if( pthread_cond_init(&m_cond, NULL) != 0
        || pthread_mutex_init(&m_mutex, NULL) != 0 )
    {
        // LOG_ERROR("Init()���� [%s]", strerror((int)error) );
        return ERR;
    }

    return OK;
}

int CondLock::Wait()
{
    FUNCTION_TRACK(); // �����켣����

    pthread_mutex_lock(&m_mutex);

    /*
     * �ȴ���������ʱ��ʽ��
     * ע�⣺�鿴����[man pthread_cond_timedwait]����˵����pthread_cond_timedwait()��
     *       ��һ�����յ��źţ��������Ǽٵģ�����Ҫ�ٴμ�����������������ǵ�ϵͳ��˵
     *       ���غ��粻���ݣ�Ҳ��Ӱ���������С���ͬ��   [XXX]
     */
    struct timespec t;
    t.tv_sec = time(NULL) + 5; // ע�����ﳬʱʹ�õ���ʱ��㣨����ʱ��Σ��������ˡ�[XXX] <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    t.tv_nsec = 0;
    pthread_cond_timedwait(&m_cond, &m_mutex, &t); // ��ʱ��ʽ
    // pthread_cond_wait(&m_cond, &m_mutex);

    pthread_mutex_unlock(&m_mutex);

    return OK;
}

int CondLock::Wake()
{
    FUNCTION_TRACK(); // �����켣����

    int ret = pthread_cond_broadcast(&m_cond);  // �㲥��ʽ
    // LOG_DEBUG("����֪ͨ��pthread_cond_signal()����: %d", ret);

    return OK;
}















}// end of CONDLOCK_SPACE

