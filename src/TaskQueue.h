#ifndef __TASKQUEUE_H_20100426__
#define __TASKQUEUE_H_20100426__
#include "Common.h"
#include "RingQueueOfInt.h"
namespace TASKQUEUE_SPACE
{





// 线程池处理任务队列
class TaskQueue
{
public:
    // max: 最大任务数
    TaskQueue(int max);

    //
    int Init();

    // 放入一个任务
    int push(void *task);

    // 取出一个任务
    void *pop();

    // 当前队列任务数
    int size();

private:
    RingQueueOfInt m_queue; // 环形存储队列
    Lock m_PushLock;        // 写队列锁
    Lock m_PopLock;         // 读队列锁（注意，这里的读、写锁是无关联的）
};






}// end of TASKQUEUE_SPACE
using namespace TASKQUEUE_SPACE;
#endif // end of __TASKQUEUE_H_20100426__
