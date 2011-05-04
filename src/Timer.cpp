// Rocky 2010-05-25 11:39:25
#include "Timer.h"
namespace TIMER_SPACE
{









Timer::Timer(): m_exit(false)
{
    // 启动执行线程
    boost::thread t( boost::bind(&Run, this) );
    // 设置为分离状态
    t.detach();
}

Timer::~Timer()
{
    // 使定时线程退出
    m_exit = true;
}

// 类初始化（的程序启动时执行一次）
int Timer::init()
{
    static Timer timer;

    // 注册到全局环境记录
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
 * 删除任务。注：由于可能在任务执行函数内部调注销（删除）操作，因此这里并
 * 不执行真正删除，只置为无效标记，而稍后调ClearInvalid()才真正从队列中删
 * 除任务；
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
    it->invalid = true; // 标记为无效
    LOG_DEBUG("Delete task: [%p]", task.exector);
    return OK;
}

// 清除无效任务
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
            // 对list的删除操作，小心；
            it = m_queue.erase(it);
        }
        else
        {
            it++;
        }
    }
    return OK;
}

// 任务主循环
void Timer::Run(Timer *timer)
{
    assert(NULL != timer);
    LOG_INFO("Starting timer: [%p]", timer);

    while( ! timer->m_exit )
    {
        {// 执行每个登记任务（大括号用于自动加、解锁）
            SHARE_LOCK(timer->m_lock);
            list<Task>::iterator it;
            for(it = timer->m_queue.begin(); (!timer->m_exit && timer->m_queue.end() != it); it++)
            {
                Task &task = *it;

                // 跳过无效项
                if(task.invalid)
                {
                    continue;
                }

                LOG_DEBUG("Execute task: [%p] [%s]", task.exector, task.remark.c_str());
                // 执行任务
                (*task.exector)(task.param);
            }
        }

        sleep(10); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<< 应可配置

        // 清理无效项
        timer->ClearInvalid();
    }
}

// 注册需要定时执行的任务
int Timer::Register(const ExeFunc exector, void *param, const string &remark)
{
    Task task;
    task.exector = exector;
    task.param = param;
    task.remark = remark;
    return instance()->Insert(task);
}

// 注销定时任务
int Timer::UnRegister(const ExeFunc exector)
{
    return instance()->Delete(exector);
}









}// end of TIMER_SPACE
