#ifndef __CONNECTQUEUE_H_20100414__
#define __CONNECTQUEUE_H_20100414__
#include "Common.h"
#include "Connect.h"
#include "CondLock.h"
#include "RingQueueOfInt.h"
namespace CONNECTQUEUE_SPACE
{




// ���Ӷ��У���ģʽ��
class ConnectQueue
{
public:
    // ��ʼ�������价�ζ����ڴ�ȣ�
    static int init();

    // ����()������������ģ������ַ��
    inline static ConnectQueue * instance()
    {
        FUNCTION_TRACK(); // �����켣����
        static ConnectQueue s;
        return &s;
    }

    // ���Ӷ���ָ�룩�������
    int push(Connect *&connect);

    // ȡ�����Ӷ���ͬʱ�Ѹö���Ӷ�����ɾ����
    Connect *pop();

private:
    ConnectQueue();

private:
    RingQueueOfInt m_queue; // ���Ӷ���
    CondLock m_lock;        // �������������������ݴ���ʱ�����Ѵ����̣߳�
};














}// end of CONNECTQUEUE_SPACE
using namespace CONNECTQUEUE_SPACE;
#endif // end of __CONNECTQUEUE_H_20100414__
