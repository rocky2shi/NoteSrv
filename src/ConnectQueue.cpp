// Rocky 2010-04-14 19:28:48
#include "ConnectQueue.h"
namespace CONNECTQUEUE_SPACE
{




ConnectQueue::ConnectQueue() : m_queue( 100000 )     // 100000为同存放放连接数上限
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    // 初始化（分配环形队列内存等）
    if( m_queue.Init() < 0 )
    {
        LOG_ERROR("严重错误：初始化连接队列出错!!!!!!!!!!!!!!!!!!");
        exit(ERR);
    }
}


int ConnectQueue::init()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    ConnectQueue::instance();
}

// 连接对象（指针）加入队列
int ConnectQueue::push(Connect *&connect)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    // 把“指针”存入队列
    if( m_queue.Push( connect ) < 0 )
    {
        return ERR;
    }

    m_lock.Wake(); // 唤醒处理线程
    return OK;
}

// 取出连接对象（同时把该对象从队列中删除）
Connect *ConnectQueue::pop()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    if( m_queue.isEmpty() )
    {
        m_lock.Wait();  // 无数据，等待（超时形式）；
    }
    void *element = NULL;
    m_queue.Pop( element );
    return (Connect *)element;
}









}// end of CONNECTQUEUE_SPACE

