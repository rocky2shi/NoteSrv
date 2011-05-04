#ifndef __THREADPOOL_H_20100426__
#define __THREADPOOL_H_20100426__
#include "Common.h"
#include "CondLock.h"
#include "TaskQueue.h"
#include <signal.h> // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 考滤多平台
namespace THREADPOOL_SPACE
{






// 线程池
class ThreadPool
{
    static const int THREAD_COUNT_MAX = 50;
    static const int THREAD_COUNT_MIN = 5;
    static const int TASK_COUNT_MAX = 2000; // 任务队列容量（任务个数）[XXX]
    typedef void *(*Func)(void *);
    // 线程状态结构
    struct Status {
        // 线程状态值
        typedef enum
        {
            OK,             // 正常状态
            FINISH,         // 需要退出
            NONE            // 无动作
        } STATUS;

        Status()
        {
            tid = pthread_self();
            alive = time(NULL);
            status = OK;
        }

        long tid;           // 线程号
        long alive;         // 最后活跃时间
        STATUS status;
    };

    struct StatusQueue
    {
        void add(long tid, Status *status)
        {
            UNIQUE_LOCK(m_Lock);
            m_Queue[ tid ] = status;
        }
        Status * get(long tid)
        {
            SHARE_LOCK(m_Lock);
            map<long, Status*>::iterator it = m_Queue.find(tid);
            if(m_Queue.end() == it)
            {
                return NULL;
            }
            return it->second;
        }
        int size()
        {
            SHARE_LOCK(m_Lock);
            return m_Queue.size();
        }
        // 删除一条线程状态记录（当线程即将退出时会调用）
        void del(long tid)
        {
            UNIQUE_LOCK(m_Lock);
            m_Queue.erase(tid);
        }
        // 设置任一个线程为完成态（在处理完成当前任务后，它自动退出）
        bool SetFinish()
        {
            SHARE_LOCK(m_Lock);
            map<long, Status*>::iterator it = m_Queue.begin();  // 选择队首的线程（随意选择）
            if( m_Queue.end() == it )
            {
                return false;   // 无线程
            }
            it->second->status = Status::FINISH;
            return true;
        }
        // 设置所有线程为完成态（在处理完成当前任务后，它自动退出）
        void SetFinishAll()
        {
            SHARE_LOCK(m_Lock);
            map<long, Status*>::iterator it;
            for(it = m_Queue.begin(); m_Queue.end() != it; it++)
            {
                it->second->status = Status::FINISH;
            }
        }

        // 清除无效线程记录（返回已清除数）
        int ClearInavlidStatus()
        {
            UNIQUE_LOCK(m_Lock);
            int n = 0;
            map<long, Status*>::iterator it;
            for(it = m_Queue.begin(); m_Queue.end() != it; it++)
            {
                // it->second->status = Status::FINISH;
                if( pthread_kill(it->second->tid, 0) == ESRCH )
                {
                    n++;
                    m_Queue.erase(it->second->tid);
                }
            }
            return n;
        }

    private:
        map<long, Status*> m_Queue;   // 每个元素对应一线程（tid => Status）
        Lock m_Lock;
    };


public:
    // 迭代器，用于列出所有已分配的池对象（值）；
    class iterator
    {
    public:
        iterator(): first(true)
        {
            it = ThreadPool::instance()->m_list.begin();
        }

        // 指向下一个元素
        bool next()
        {
            if( first )
            {
                first = false;
            }
            else
            {
                it++;
            }
            return it != ThreadPool::instance()->m_list.end();
        }

        // 重载操作符，返回池对象地址；
        ThreadPool * operator()()
        {
            return it->second;
        }

    private:
        map<unsigned int, ThreadPool*>::iterator it;
        bool first;
    };


private:
    // 线程管理类（记录每个分配的线程池对象）
    class Manage
    {
        friend class ThreadPool::iterator;
    public:
        Manage() : m_id(0)
        {
        }
        void Insert(ThreadPool *pool)
        {
            UNIQUE_LOCK(m_InsertLock);
            pool->m_id = GetId();
            m_list[ pool->m_id ] = pool;
            LOG_DEBUG("Manage>>> insert: [%u] [%p]", pool->m_id, pool);
        }
        void Delete(ThreadPool *pool)
        {
            UNIQUE_LOCK(m_InsertLock);
            m_list.erase( pool->m_id );
            LOG_DEBUG("Manage>>> delete: [%u] [%p]", pool->m_id, pool);
        }
    private:
        // 线程池编号
        unsigned int GetId()
        {
            return m_id++;
        }
        Lock m_InsertLock;  // 创建新类时防止冲突
        // list<ThreadPool*> m_list; // 记录每个新建（已分配）的线程池对象
        map<unsigned int, ThreadPool*> m_list; // 记录每个新建（已分配）的线程池对象
        unsigned int m_id;
    };



public:
    // func--线程池回调函数，min--线程数下限，max--线程数上限；
    ThreadPool(Func func, int min=THREAD_COUNT_MIN, int max=THREAD_COUNT_MAX);
    ~ThreadPool();

    // 类初始化
    static int init();

    // 对象初始化
    int Init();

    // 给线程池送任务数据
    int push(void *task);

    // 取当前线程池中线程数
    int GetPoolSize();

    // 取当前线程池中任务数
    int GetTaskCount();

    // 取线程池编号
    unsigned int GetId();

    // 清除无效线程记录
    int ClearInavlidThread();

private:
    // 任务主循环（循环调用外部传入的函数）
    static void Run(ThreadPool *pool);

    // 管理线程，定时扫描（所有线程池）数据队列，看看是否需要启动新线程，或释放部分空闲线程；
    static void WatchThread();

    // 单模式中用的取实例接口
    inline static Manage *instance(Manage *pool=NULL)
    {
        static Manage *obj = (Manage *)(Environment::instance()->ClassInit("ThreadPool::Manage", pool));
        return obj;
    }

private:
    StatusQueue m_StatusQueue;
    boost::thread_group m_ThreadQueue;  // 使用boost库来存放线程组
    TaskQueue m_TaskList;
    Func m_execute; // 外部传入的执行函数
    CondLock m_lock;
    bool m_bInit;   // 是否已执行过初始化
    bool m_bStopCreateThread;
    unsigned int m_id;
};











}// end of THREADPOOL_SPACE
using namespace THREADPOOL_SPACE;
#endif // end of __THREADPOOL_H_20100426__
