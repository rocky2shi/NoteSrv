// Rocky 2010-04-26 11:32:59
#include "ThreadPool.h"
#include "SignalDeal.h"
namespace THREADPOOL_SPACE
{







ThreadPool::ThreadPool(Func func, int min/*=THREAD_COUNT_MIN*/, int max/*=THREAD_COUNT_MAX*/)
                : m_TaskList(TASK_COUNT_MAX),
                  m_execute( func ),
                  m_bInit( false ),
                  m_bStopCreateThread( false ),
                  m_id( 0 )
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    // 加入管理列表
    ThreadPool::instance()->Insert( this );
    LOG_DEBUG("ThreadPool>>> insert: %u  %p", this->m_id, this);

    int i;

    // 启动工作线程（组）
    for(i=0; i<min; i++)
    {
        m_ThreadQueue.create_thread( boost::bind( &Run, this ) );
    }

    // 一般情况下，应当在外部调中执行初始化，并测试初始化是否成功；
    Init();
}

ThreadPool::~ThreadPool()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
    m_bStopCreateThread = true;
    m_StatusQueue.SetFinishAll();
    m_lock.Wake(); // 发送执行通知
    m_ThreadQueue.join_all();
    ThreadPool::instance()->Delete( this );
    LOG_DEBUG("ThreadPool>>> quit: %u  %p", this->m_id, this);
}

// 类初始化
int ThreadPool::init()
{
    // 启动管理线程
    static boost::thread t( boost::bind(&WatchThread) );

    // 管理类初始化
    static ThreadPool::Manage pool;
    Manage *obj = ThreadPool::instance( &pool );
    if(NULL == obj)
    {
        LOG_ERROR("ThreadPool::instance() error");
        return ERR;
    }

    return OK;
}

// 对象初始化
int ThreadPool::Init()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    if( m_bInit )
    {
        return OK;
    }

    int ret;

    // 初始化条件变量锁
    ret = m_lock.Init();
    if(ret < 0)
    {
        LOG_ERROR("m_lock.Init() error.");
        return ERR;
    }

    // 初始化任务队列
    ret = m_TaskList.Init();
    if(ret < 0)
    {
        LOG_ERROR("m_TaskList.Init() error.");
        return ERR;
    }

    m_bInit = true;
    return OK;
}

// 取当前线程池中线程数
int ThreadPool::GetPoolSize()
{
    return m_StatusQueue.size();
}

// 取当前线程池中任务数
int ThreadPool::GetTaskCount()
{
    return m_TaskList.size();
}

// 取线程池编号
unsigned int ThreadPool::GetId()
{
    return m_id;
}

// 清除无效线程记录
int ThreadPool::ClearInavlidThread()
{
    return m_StatusQueue.ClearInavlidStatus();
}

// 给线程池送任务数据
int ThreadPool::push(void *task)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    int ret;

    ret = m_TaskList.push( task );
    if(ret < 0)
    {
        return ERR;
    }
    usleep(10000);
    m_lock.Wake();  // 唤醒处理线程
    return OK;
}

// 任务主循环（循环调用外部传入的函数）
void ThreadPool::Run(ThreadPool *pool)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    if( pool->m_bStopCreateThread )
    {
        LOG_DEBUG("Stop create new thread.");
        return;
    }

    const long tid = pthread_self();

    /*
     * 记录当前线程信息（加入状态队列）
     */
    Status * const myself = new Status;
    if(NULL == myself)
    {
        LOG_ERROR("new Status error");
        return;
    }
    pool->m_StatusQueue.add(tid, myself);

    LOG_DEBUG("[%d]. start thread, tid=[%lu]", pool->m_StatusQueue.size(), tid);

    /*
     * 执行循环
     */
    while( !SignalDeal::exit() )
    {
        // 查看自已（当前线程）的状态
        if( Status::FINISH == myself->status )
        {
            pool->m_StatusQueue.del( tid ); // 退出前，先从状态记录队列中删除本线程信息；
            LOG_DEBUG("thread[tid:%lu] finish, quit...", tid);
            break;
        }

        //LOG_DEBUG("pool->m_TaskList.size=[%d], myself->status=[%d]", pool->m_TaskList.size(), myself->status);

        /*
         * 执行任务处理，直到任务队列空；
         */
        void *task;
        while( (task = pool->m_TaskList.pop()) != NULL )
        {
            //LOG_DEBUG("[%p]", task);
            // 执行任务
            (*pool->m_execute)( task );
        }


        /*
         * 处理完毕，进入睡眠，等待被唤醒；
         */
        pool->m_lock.Wait();
    }
}


// 管理线程，定时扫描数据队列，看看是否需要启动新线程，或释放部分空闲线程；
void ThreadPool::WatchThread()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    const unsigned int USEC_MAX = 3000000; // 3秒
    const unsigned int USEC_MIN = 200000;
    unsigned int interval = USEC_MAX;

    while( !SignalDeal::exit() )
    {
        if( interval > USEC_MAX )
        {
            interval = USEC_MAX;
        }
        else if( interval < USEC_MIN )
        {
            interval = USEC_MIN;
        }

        usleep( interval );   // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 应可配制

        // LOG_DEBUG("m_PoolList.size=[%d]", m_PoolList.size());

        ThreadPool::iterator it;

        // 扫描各个程序池对象
        while( it.next() )
        {
            ThreadPool * const pool = it();

            //LOG_DEBUG("[%d] [%d] [%d]", pool->GetPoolSize(), pool->GetTaskCount(), THREAD_COUNT_MAX);

            if( pool->m_bStopCreateThread )
            {
                LOG_DEBUG("Stop create new thread.");
                return;
            }

            int ret = pool->ClearInavlidThread();
            //LOG_DEBUG("Clear inavlid thread number: %d", ret);


            /* 检查当前任务是否繁忙，可能需要创建更多的处理线程；
             *
             *      当：1. 务任个数 > 线程数，且
             *          2. 线程数不到达最大限制时
             *
             *      创建一新线程；
             */
            if( pool->m_TaskList.size() > pool->m_StatusQueue.size()
                && pool->m_StatusQueue.size() <= THREAD_COUNT_MAX
              )
            {
                // 创建一新线程
                pool->m_ThreadQueue.create_thread( boost::bind( &ThreadPool::Run, pool ) );
                interval -= 200000; // 在任务忙时，应快速启动新线程；
            }

            /*
             * 空闲线程过多时，命令多余线程退出；
             */
            if( pool->m_TaskList.size() < pool->m_StatusQueue.size()
                && pool->m_StatusQueue.size() > THREAD_COUNT_MIN
              )
            {
                pool->m_StatusQueue.SetFinish(); // 设置任一条线程为完成态；
                pool->m_lock.Wake(); // 发送执行通知
                interval += 300000;
            }
        }// while( it.next()...
    }// end of while(1)...
}













}// end of THREADPOOL_SPACE
