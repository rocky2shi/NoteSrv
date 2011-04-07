#ifndef __LOCK_H_20100319__
#define __LOCK_H_20100319__
#include <boost/thread.hpp>
namespace LOCK_SPACE
{




// 使用boost库，包装共享-独占锁；
class Lock
{
public:
    // 共享锁
    class Share
    {
    public:
        Share(Lock &lock) : m_lock(lock)
        {
            m_lock.m_mutex.lock_shared();
        }
        ~Share()
        {
            m_lock.m_mutex.unlock_shared();
        }
    private:
        Lock &m_lock;
    };

    // 独占锁
    class Unique
    {
    public:
        Unique(Lock &lock) : m_lock(lock)
        {
            m_lock.m_mutex.lock();
        }
        ~Unique()
        {
            m_lock.m_mutex.unlock();
        }
    private:
        Lock &m_lock;
    };

public:
    boost::shared_mutex m_mutex;
};

#define SHARE_LOCK(obj)  Lock::Share LOCK_TMP(obj)
#define UNIQUE_LOCK(obj) Lock::Unique LOCK_TMP(obj)












}// end of LOCK_SPACE
using namespace LOCK_SPACE;
#endif // end of __LOCK_H_20100319__
