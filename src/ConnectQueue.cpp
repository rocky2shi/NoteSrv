// Rocky 2010-04-14 19:28:48
#include "ConnectQueue.h"
namespace CONNECTQUEUE_SPACE
{




ConnectQueue::ConnectQueue() : m_queue( 100000 )     // 100000Ϊͬ��ŷ�����������
{
    FUNCTION_TRACK(); // �����켣����

    // ��ʼ�������价�ζ����ڴ�ȣ�
    if( m_queue.Init() < 0 )
    {
        LOG_ERROR("���ش��󣺳�ʼ�����Ӷ��г���!!!!!!!!!!!!!!!!!!");
        exit(ERR);
    }
}


int ConnectQueue::init()
{
    FUNCTION_TRACK(); // �����켣����
    ConnectQueue::instance();
}

// ���Ӷ���ָ�룩�������
int ConnectQueue::push(Connect *&connect)
{
    FUNCTION_TRACK(); // �����켣����

    // �ѡ�ָ�롱�������
    if( m_queue.Push( connect ) < 0 )
    {
        return ERR;
    }

    m_lock.Wake(); // ���Ѵ����߳�
    return OK;
}

// ȡ�����Ӷ���ͬʱ�Ѹö���Ӷ�����ɾ����
Connect *ConnectQueue::pop()
{
    FUNCTION_TRACK(); // �����켣����

    if( m_queue.isEmpty() )
    {
        m_lock.Wait();  // �����ݣ��ȴ�����ʱ��ʽ����
    }
    void *element = NULL;
    m_queue.Pop( element );
    return (Connect *)element;
}









}// end of CONNECTQUEUE_SPACE

