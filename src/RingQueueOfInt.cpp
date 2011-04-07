// Rocky 2010-04-26 18:10:44
#include "RingQueueOfInt.h"
#include <stdlib.h>
namespace RINGQUEUEOFINT_SPACE
{








RingQueueOfInt::RingQueueOfInt(int max)
                    : m_max(max + 1),
                      m_buf( (void **)(malloc( m_max * sizeof(void *) ) ) ),
                      m_read(0),
                      m_write(0)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}

RingQueueOfInt::~RingQueueOfInt()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
    free(m_buf);
}

//
int RingQueueOfInt::Init()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
    return NULL != m_buf ? OK : ERR;
}

// 住队列中存入一个元素
int RingQueueOfInt::Push(void *element)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    if( isFull() )
    {
        // LOG_DEBUG("Queue full.");
        return ERR;
    }

    m_buf[ m_write ] = element;
    m_write++;
    if(m_max == m_write)
    {
        m_write = 0;
    }

    return OK;
}

// 取一个元素
int RingQueueOfInt::Pop(void *&element)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪

    if( isEmpty() )
    {
        //LOG_DEBUG("Queue empty.");
        return ERR;
    }

    element = m_buf[ m_read ];
    m_read++;
    if(m_max == m_read)
    {
        m_read = 0;
    }

    return OK;
}

// 当前队列元素个数
int RingQueueOfInt::Size()
{
    // FUNCTION_TRACK(); // 函数轨迹跟踪

    if( m_write >= m_read )
    {
        // 读指针在前，直接求差值；
        return m_write - m_read;
    }
    else
    {
        // 写指针在前，则要计算“回饶”；
        return m_write - m_read + m_max;
    }
}

// 队列满返回true
bool RingQueueOfInt::isFull()
{
    // FUNCTION_TRACK(); // 函数轨迹跟踪
    /*
     * 满需分情况处理：
     */
    if( m_write + 1 == m_read)  // 1. 写 < 读
    {
        return true;
    }
    else if( 0 == m_read && (m_write + 1) == m_max) // 2. 读 < 写
    {
        return true;
    }
    return false;
}

// 队列空返回true
bool RingQueueOfInt::isEmpty()
{
    // FUNCTION_TRACK(); // 函数轨迹跟踪
    return m_read == m_write;
}















}// end of RINGQUEUEOFINT_SPACE
