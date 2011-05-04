#ifndef __CONNECTQUEUE_H_20100414__
#define __CONNECTQUEUE_H_20100414__
#include "Common.h"
#include "Connect.h"
#include "CondLock.h"
#include "RingQueueOfInt.h"
namespace CONNECTQUEUE_SPACE
{




// 连接队列（单模式）
class ConnectQueue
{
public:
    // 初始化（分配环形队列内存等）
    static int init();

    // 重载()操作符，返回模块对象地址；
    inline static ConnectQueue * instance()
    {
        FUNCTION_TRACK(); // 函数轨迹跟综
        static ConnectQueue s;
        return &s;
    }

    // 连接对象（指针）加入队列
    int push(Connect *&connect);

    // 取出连接对象（同时把该对象从队列中删除）
    Connect *pop();

private:
    ConnectQueue();

private:
    RingQueueOfInt m_queue; // 连接队列
    CondLock m_lock;        // 条件变量锁，当有数据处理时，唤醒处理线程；
};














}// end of CONNECTQUEUE_SPACE
using namespace CONNECTQUEUE_SPACE;
#endif // end of __CONNECTQUEUE_H_20100414__
