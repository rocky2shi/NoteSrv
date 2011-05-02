#ifndef __RINGQUEUEOFINT_H_20100426__
#define __RINGQUEUEOFINT_H_20100426__
#include "Common.h"
namespace RINGQUEUEOFINT_SPACE
{





// ���ζ��У�ÿԪ��Ϊһ���������ȣ�
class RingQueueOfInt
{
public:
    RingQueueOfInt(int max);
    ~RingQueueOfInt();

    //
    int Init();

    // ס�����д���һ��Ԫ�أ����ܴ��룬����С��0ֵ��
    int Push(void *element);

    // ȡһ��Ԫ��
    int Pop(void *&element);

    // ��ǰ����Ԫ�ظ���
    int Size();

    // ����������true
    bool isFull();

    // ���пշ���true
    bool isEmpty();


private:
    static const int DEFAULT_COUNT_MAX = 500;
    const int m_max;

    void ** const m_buf;    // ���ݴ�Ŵ�
    int m_read;             // ��λ��
    int m_write;            // дλ��
};






}// end of RINGQUEUEOFINT_SPACE
using namespace RINGQUEUEOFINT_SPACE;
#endif // end of __RINGQUEUEOFINT_H_20100426__
