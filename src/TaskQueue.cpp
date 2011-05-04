// Rocky 2010-04-26 15:26:46
#include "TaskQueue.h"
namespace TASKQUEUE_SPACE
{





// max: 最大任务数
TaskQueue::TaskQueue(int max) : m_queue(max)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

int TaskQueue::Init()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return m_queue.Init();
}

// 放入一个任务（可能有多线程操作，须加锁）
int TaskQueue::push(void *task)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    UNIQUE_LOCK(m_PushLock);

    return m_queue.Push( task );
}

// 取出一个任务（有多线程操作，须加锁）
void *TaskQueue::pop()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    UNIQUE_LOCK(m_PopLock);

    void *element = NULL;
    m_queue.Pop(element);
    return element;
}

// 当前队列任务数
int TaskQueue::size()
{
    //FUNCTION_TRACK(); // 函数轨迹跟综
    return m_queue.Size();
}




}// end of TASKQUEUE_SPACE
