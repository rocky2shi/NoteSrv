#ifndef __CONDLOCK_H_20100425__
#define __CONDLOCK_H_20100425__
#include "Common.h"
namespace CONDLOCK_SPACE
{







// 条件锁
class CondLock
{
public:
    //
    int Init();

    // 锁（等待）
    int Wait();

    // 解锁（唤醒等待，广播形式）
    int Wake();


private:
    pthread_cond_t m_cond;      // 这里暂只支持linux和实现，等改。[XXX]
    pthread_mutex_t m_mutex;
};





}// end of CONDLOCK_SPACE
using namespace CONDLOCK_SPACE;
#endif // end of __CONDLOCK_H_20100425__
