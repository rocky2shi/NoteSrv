// Rocky 2010-04-25 15:18:39
#include "CondLock.h"
#include <error.h>
namespace CONDLOCK_SPACE
{




int CondLock::Init()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    if( pthread_cond_init(&m_cond, NULL) != 0
        || pthread_mutex_init(&m_mutex, NULL) != 0 )
    {
        // LOG_ERROR("Init()出错 [%s]", strerror((int)error) );
        return ERR;
    }

    return OK;
}

int CondLock::Wait()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    pthread_mutex_lock(&m_mutex);

    /*
     * 等待条件（超时形式）
     * 注意：查看帮助[man pthread_cond_timedwait]，有说明，pthread_cond_timedwait()返
     *       不一定是收到信号，它可能是假的，所以要再次检查条件，但对于我们的系统来说
     *       返回后如不数据，也不影响正常运行。下同。   [XXX]
     */
    struct timespec t;
    t.tv_sec = time(NULL) + 5; // 注意这里超时使用的是时间点（不是时间段），搞死了。[XXX] <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    t.tv_nsec = 0;
    pthread_cond_timedwait(&m_cond, &m_mutex, &t); // 超时形式
    // pthread_cond_wait(&m_cond, &m_mutex);

    pthread_mutex_unlock(&m_mutex);

    return OK;
}

int CondLock::Wake()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    int ret = pthread_cond_broadcast(&m_cond);  // 广播形式
    // LOG_DEBUG("发送通知函pthread_cond_signal()返回: %d", ret);

    return OK;
}















}// end of CONDLOCK_SPACE

