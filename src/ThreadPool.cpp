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
    FUNCTION_TRACK(); // �����켣����

    // ��������б�
    ThreadPool::instance()->Insert( this );
    LOG_DEBUG("ThreadPool>>> insert: %u  %p", this->m_id, this);

    int i;

    // ���������̣߳��飩
    for(i=0; i<min; i++)
    {
        m_ThreadQueue.create_thread( boost::bind( &Run, this ) );
    }

    // һ������£�Ӧ�����ⲿ����ִ�г�ʼ���������Գ�ʼ���Ƿ�ɹ���
    Init();
}

ThreadPool::~ThreadPool()
{
    FUNCTION_TRACK(); // �����켣����
    m_bStopCreateThread = true;
    m_StatusQueue.SetFinishAll();
    m_lock.Wake(); // ����ִ��֪ͨ
    m_ThreadQueue.join_all();
    ThreadPool::instance()->Delete( this );
    LOG_DEBUG("ThreadPool>>> quit: %u  %p", this->m_id, this);
}

// ���ʼ��
int ThreadPool::init()
{
    // ���������߳�
    static boost::thread t( boost::bind(&WatchThread) );

    // �������ʼ��
    static ThreadPool::Manage pool;
    Manage *obj = ThreadPool::instance( &pool );
    if(NULL == obj)
    {
        LOG_ERROR("ThreadPool::instance() error");
        return ERR;
    }

    return OK;
}

// �����ʼ��
int ThreadPool::Init()
{
    FUNCTION_TRACK(); // �����켣����

    if( m_bInit )
    {
        return OK;
    }

    int ret;

    // ��ʼ������������
    ret = m_lock.Init();
    if(ret < 0)
    {
        LOG_ERROR("m_lock.Init() error.");
        return ERR;
    }

    // ��ʼ���������
    ret = m_TaskList.Init();
    if(ret < 0)
    {
        LOG_ERROR("m_TaskList.Init() error.");
        return ERR;
    }

    m_bInit = true;
    return OK;
}

// ȡ��ǰ�̳߳����߳���
int ThreadPool::GetPoolSize()
{
    return m_StatusQueue.size();
}

// ȡ��ǰ�̳߳���������
int ThreadPool::GetTaskCount()
{
    return m_TaskList.size();
}

// ȡ�̳߳ر��
unsigned int ThreadPool::GetId()
{
    return m_id;
}

// �����Ч�̼߳�¼
int ThreadPool::ClearInavlidThread()
{
    return m_StatusQueue.ClearInavlidStatus();
}

// ���̳߳�����������
int ThreadPool::push(void *task)
{
    FUNCTION_TRACK(); // �����켣����

    int ret;

    ret = m_TaskList.push( task );
    if(ret < 0)
    {
        return ERR;
    }
    usleep(10000);
    m_lock.Wake();  // ���Ѵ����߳�
    return OK;
}

// ������ѭ����ѭ�������ⲿ����ĺ�����
void ThreadPool::Run(ThreadPool *pool)
{
    FUNCTION_TRACK(); // �����켣����

    if( pool->m_bStopCreateThread )
    {
        LOG_DEBUG("Stop create new thread.");
        return;
    }

    const long tid = pthread_self();

    /*
     * ��¼��ǰ�߳���Ϣ������״̬���У�
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
     * ִ��ѭ��
     */
    while( !SignalDeal::exit() )
    {
        // �鿴���ѣ���ǰ�̣߳���״̬
        if( Status::FINISH == myself->status )
        {
            pool->m_StatusQueue.del( tid ); // �˳�ǰ���ȴ�״̬��¼������ɾ�����߳���Ϣ��
            LOG_DEBUG("thread[tid:%lu] finish, quit...", tid);
            break;
        }

        //LOG_DEBUG("pool->m_TaskList.size=[%d], myself->status=[%d]", pool->m_TaskList.size(), myself->status);

        /*
         * ִ��������ֱ��������пգ�
         */
        void *task;
        while( (task = pool->m_TaskList.pop()) != NULL )
        {
            //LOG_DEBUG("[%p]", task);
            // ִ������
            (*pool->m_execute)( task );
        }


        /*
         * ������ϣ�����˯�ߣ��ȴ������ѣ�
         */
        pool->m_lock.Wait();
    }
}


// �����̣߳���ʱɨ�����ݶ��У������Ƿ���Ҫ�������̣߳����ͷŲ��ֿ����̣߳�
void ThreadPool::WatchThread()
{
    FUNCTION_TRACK(); // �����켣����

    const unsigned int USEC_MAX = 3000000; // 3��
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

        usleep( interval );   // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Ӧ������

        // LOG_DEBUG("m_PoolList.size=[%d]", m_PoolList.size());

        ThreadPool::iterator it;

        // ɨ���������ض���
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


            /* ��鵱ǰ�����Ƿ�æ��������Ҫ��������Ĵ����̣߳�
             *
             *      ����1. ���θ��� > �߳�������
             *          2. �߳����������������ʱ
             *
             *      ����һ���̣߳�
             */
            if( pool->m_TaskList.size() > pool->m_StatusQueue.size()
                && pool->m_StatusQueue.size() <= THREAD_COUNT_MAX
              )
            {
                // ����һ���߳�
                pool->m_ThreadQueue.create_thread( boost::bind( &ThreadPool::Run, pool ) );
                interval -= 200000; // ������æʱ��Ӧ�����������̣߳�
            }

            /*
             * �����̹߳���ʱ����������߳��˳���
             */
            if( pool->m_TaskList.size() < pool->m_StatusQueue.size()
                && pool->m_StatusQueue.size() > THREAD_COUNT_MIN
              )
            {
                pool->m_StatusQueue.SetFinish(); // ������һ���߳�Ϊ���̬��
                pool->m_lock.Wake(); // ����ִ��֪ͨ
                interval += 300000;
            }
        }// while( it.next()...
    }// end of while(1)...
}













}// end of THREADPOOL_SPACE
