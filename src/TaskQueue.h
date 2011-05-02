#ifndef __TASKQUEUE_H_20100426__
#define __TASKQUEUE_H_20100426__
#include "Common.h"
#include "RingQueueOfInt.h"
namespace TASKQUEUE_SPACE
{





// �̳߳ش����������
class TaskQueue
{
public:
    // max: ���������
    TaskQueue(int max);

    //
    int Init();

    // ����һ������
    int push(void *task);

    // ȡ��һ������
    void *pop();

    // ��ǰ����������
    int size();

private:
    RingQueueOfInt m_queue; // ���δ洢����
    Lock m_PushLock;        // д������
    Lock m_PopLock;         // ����������ע�⣬����Ķ���д�����޹����ģ�
};






}// end of TASKQUEUE_SPACE
using namespace TASKQUEUE_SPACE;
#endif // end of __TASKQUEUE_H_20100426__
