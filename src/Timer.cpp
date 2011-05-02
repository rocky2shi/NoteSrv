// Rocky 2010-05-25 11:39:25
#include "Timer.h"
namespace TIMER_SPACE
{









Timer::Timer(): m_exit(false)
{
    // ����ִ���߳�
    boost::thread t( boost::bind(&Run, this) );
    // ����Ϊ����״̬
    t.detach();
}

Timer::~Timer()
{
    // ʹ��ʱ�߳��˳�
    m_exit = true;
}

// ���ʼ�����ĳ�������ʱִ��һ�Σ�
int Timer::init()
{
    static Timer timer;

    // ע�ᵽȫ�ֻ�����¼
    Timer *obj = instance( &timer );
    if(NULL == obj)
    {
        LOG_ERROR("Timer::instance() error.");
        return ERR;
    }

    return OK;
}

int Timer::Insert(const Task &task)
{
    UNIQUE_LOCK(m_lock);
    m_queue.insert(m_queue.begin(), Task(task));
    LOG_DEBUG("Insert task: [%p]", task.exector);
    return OK;
}

/*
 * ɾ������ע�����ڿ���������ִ�к����ڲ���ע����ɾ����������������ﲢ
 * ��ִ������ɾ����ֻ��Ϊ��Ч��ǣ����Ժ��ClearInvalid()�������Ӷ�����ɾ
 * ������
 */
int Timer::Delete(const ExeFunc exector)
{
    SHARE_LOCK(m_lock);

    Task task;
    list<Task>::iterator it;
    task.exector = exector;
    it = std::find(m_queue.begin(), m_queue.end(), task);
    if(m_queue.end() == it)
    {
        LOG_ERROR("Task not exist: [%p] [%s]", task.exector, task.remark.c_str());
        return ERR;
    }
    // m_queue.erase(it);
    it->invalid = true; // ���Ϊ��Ч
    LOG_DEBUG("Delete task: [%p]", task.exector);
    return OK;
}

// �����Ч����
int Timer::ClearInvalid()
{
    UNIQUE_LOCK(m_lock);

    list<Task>::iterator it;
    for(it = m_queue.begin(); (!m_exit && m_queue.end() != it); )
    {
        Task &task = *it;
        if(task.invalid)
        {
            LOG_DEBUG("Clear invalid task: [%p] [%s]", task.exector, task.remark.c_str());
            // ��list��ɾ��������С�ģ�
            it = m_queue.erase(it);
        }
        else
        {
            it++;
        }
    }
    return OK;
}

// ������ѭ��
void Timer::Run(Timer *timer)
{
    assert(NULL != timer);
    LOG_INFO("Starting timer: [%p]", timer);

    while( ! timer->m_exit )
    {
        {// ִ��ÿ���Ǽ����񣨴����������Զ��ӡ�������
            SHARE_LOCK(timer->m_lock);
            list<Task>::iterator it;
            for(it = timer->m_queue.begin(); (!timer->m_exit && timer->m_queue.end() != it); it++)
            {
                Task &task = *it;

                // ������Ч��
                if(task.invalid)
                {
                    continue;
                }

                LOG_DEBUG("Execute task: [%p] [%s]", task.exector, task.remark.c_str());
                // ִ������
                (*task.exector)(task.param);
            }
        }

        sleep(10); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<< Ӧ������

        // ������Ч��
        timer->ClearInvalid();
    }
}

// ע����Ҫ��ʱִ�е�����
int Timer::Register(const ExeFunc exector, void *param, const string &remark)
{
    Task task;
    task.exector = exector;
    task.param = param;
    task.remark = remark;
    return instance()->Insert(task);
}

// ע����ʱ����
int Timer::UnRegister(const ExeFunc exector)
{
    return instance()->Delete(exector);
}









}// end of TIMER_SPACE
