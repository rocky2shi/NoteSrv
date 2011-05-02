// Rocky 2010-04-26 15:26:46
#include "TaskQueue.h"
namespace TASKQUEUE_SPACE
{





// max: ���������
TaskQueue::TaskQueue(int max) : m_queue(max)
{
    FUNCTION_TRACK(); // �����켣����
}

int TaskQueue::Init()
{
    FUNCTION_TRACK(); // �����켣����
    return m_queue.Init();
}

// ����һ�����񣨿����ж��̲߳������������
int TaskQueue::push(void *task)
{
    FUNCTION_TRACK(); // �����켣����

    UNIQUE_LOCK(m_PushLock);

    return m_queue.Push( task );
}

// ȡ��һ�������ж��̲߳������������
void *TaskQueue::pop()
{
    FUNCTION_TRACK(); // �����켣����

    UNIQUE_LOCK(m_PopLock);

    void *element = NULL;
    m_queue.Pop(element);
    return element;
}

// ��ǰ����������
int TaskQueue::size()
{
    //FUNCTION_TRACK(); // �����켣����
    return m_queue.Size();
}




}// end of TASKQUEUE_SPACE
