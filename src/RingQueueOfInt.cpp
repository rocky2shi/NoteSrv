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
    FUNCTION_TRACK(); // �����켣����
}

RingQueueOfInt::~RingQueueOfInt()
{
    FUNCTION_TRACK(); // �����켣����
    free(m_buf);
}

//
int RingQueueOfInt::Init()
{
    FUNCTION_TRACK(); // �����켣����
    return NULL != m_buf ? OK : ERR;
}

// ס�����д���һ��Ԫ��
int RingQueueOfInt::Push(void *element)
{
    FUNCTION_TRACK(); // �����켣����

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

// ȡһ��Ԫ��
int RingQueueOfInt::Pop(void *&element)
{
    FUNCTION_TRACK(); // �����켣����

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

// ��ǰ����Ԫ�ظ���
int RingQueueOfInt::Size()
{
    // FUNCTION_TRACK(); // �����켣����

    if( m_write >= m_read )
    {
        // ��ָ����ǰ��ֱ�����ֵ��
        return m_write - m_read;
    }
    else
    {
        // дָ����ǰ����Ҫ���㡰���ġ���
        return m_write - m_read + m_max;
    }
}

// ����������true
bool RingQueueOfInt::isFull()
{
    // FUNCTION_TRACK(); // �����켣����
    /*
     * ������������
     */
    if( m_write + 1 == m_read)  // 1. д < ��
    {
        return true;
    }
    else if( 0 == m_read && (m_write + 1) == m_max) // 2. �� < д
    {
        return true;
    }
    return false;
}

// ���пշ���true
bool RingQueueOfInt::isEmpty()
{
    // FUNCTION_TRACK(); // �����켣����
    return m_read == m_write;
}















}// end of RINGQUEUEOFINT_SPACE
