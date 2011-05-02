#ifndef __THREADPOOL_H_20100426__
#define __THREADPOOL_H_20100426__
#include "Common.h"
#include "CondLock.h"
#include "TaskQueue.h"
#include <signal.h> // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ���˶�ƽ̨
namespace THREADPOOL_SPACE
{






// �̳߳�
class ThreadPool
{
    static const int THREAD_COUNT_MAX = 50;
    static const int THREAD_COUNT_MIN = 5;
    static const int TASK_COUNT_MAX = 2000; // ����������������������[XXX]
    typedef void *(*Func)(void *);
    // �߳�״̬�ṹ
    struct Status {
        // �߳�״ֵ̬
        typedef enum
        {
            OK,             // ����״̬
            FINISH,         // ��Ҫ�˳�
            NONE            // �޶���
        } STATUS;

        Status()
        {
            tid = pthread_self();
            alive = time(NULL);
            status = OK;
        }

        long tid;           // �̺߳�
        long alive;         // ����Ծʱ��
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
        // ɾ��һ���߳�״̬��¼�����̼߳����˳�ʱ����ã�
        void del(long tid)
        {
            UNIQUE_LOCK(m_Lock);
            m_Queue.erase(tid);
        }
        // ������һ���߳�Ϊ���̬���ڴ�����ɵ�ǰ��������Զ��˳���
        bool SetFinish()
        {
            SHARE_LOCK(m_Lock);
            map<long, Status*>::iterator it = m_Queue.begin();  // ѡ����׵��̣߳�����ѡ��
            if( m_Queue.end() == it )
            {
                return false;   // ���߳�
            }
            it->second->status = Status::FINISH;
            return true;
        }
        // ���������߳�Ϊ���̬���ڴ�����ɵ�ǰ��������Զ��˳���
        void SetFinishAll()
        {
            SHARE_LOCK(m_Lock);
            map<long, Status*>::iterator it;
            for(it = m_Queue.begin(); m_Queue.end() != it; it++)
            {
                it->second->status = Status::FINISH;
            }
        }

        // �����Ч�̼߳�¼���������������
        int ClearInavlidStatus()
        {
            SHARE_LOCK(m_Lock);
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
        map<long, Status*> m_Queue;   // ÿ��Ԫ�ض�Ӧһ�̣߳�tid => Status��
        Lock m_Lock;
    };


public:
    // �������������г������ѷ���ĳض���ֵ����
    class iterator
    {
    public:
        iterator(): first(true)
        {
            it = ThreadPool::instance()->m_list.begin();
        }

        // ָ����һ��Ԫ��
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

        // ���ز����������سض����ַ��
        ThreadPool * operator()()
        {
            return it->second;
        }

    private:
        map<unsigned int, ThreadPool*>::iterator it;
        bool first;
    };


private:
    // �̹߳����ࣨ��¼ÿ��������̳߳ض���
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
        // �̳߳ر��
        unsigned int GetId()
        {
            return m_id++;
        }
        Lock m_InsertLock;  // ��������ʱ��ֹ��ͻ
        // list<ThreadPool*> m_list; // ��¼ÿ���½����ѷ��䣩���̳߳ض���
        map<unsigned int, ThreadPool*> m_list; // ��¼ÿ���½����ѷ��䣩���̳߳ض���
        unsigned int m_id;
    };



public:
    // func--�̳߳ػص�������min--�߳������ޣ�max--�߳������ޣ�
    ThreadPool(Func func, int min=THREAD_COUNT_MIN, int max=THREAD_COUNT_MAX);
    ~ThreadPool();

    // ���ʼ��
    static int init();

    // �����ʼ��
    int Init();

    // ���̳߳�����������
    int push(void *task);

    // ȡ��ǰ�̳߳����߳���
    int GetPoolSize();

    // ȡ��ǰ�̳߳���������
    int GetTaskCount();

    // ȡ�̳߳ر��
    unsigned int GetId();

    // �����Ч�̼߳�¼
    int ClearInavlidThread();

private:
    // ������ѭ����ѭ�������ⲿ����ĺ�����
    static void Run(ThreadPool *pool);

    // �����̣߳���ʱɨ�裨�����̳߳أ����ݶ��У������Ƿ���Ҫ�������̣߳����ͷŲ��ֿ����̣߳�
    static void WatchThread();

    // ��ģʽ���õ�ȡʵ���ӿ�
    inline static Manage *instance(Manage *pool=NULL)
    {
        static Manage *obj = (Manage *)(Environment::instance()->ClassInit("ThreadPool::Manage", pool));
        return obj;
    }

private:
    StatusQueue m_StatusQueue;
    boost::thread_group m_ThreadQueue;  // ʹ��boost��������߳���
    TaskQueue m_TaskList;
    Func m_execute; // �ⲿ�����ִ�к���
    CondLock m_lock;
    bool m_bInit;   // �Ƿ���ִ�й���ʼ��
    bool m_bStopCreateThread;
    unsigned int m_id;
};











}// end of THREADPOOL_SPACE
using namespace THREADPOOL_SPACE;
#endif // end of __THREADPOOL_H_20100426__
