#ifndef __RINGQUEUEOFINT_H_20100426__
#define __RINGQUEUEOFINT_H_20100426__
#include "Common.h"
namespace RINGQUEUEOFINT_SPACE
{





// 环形队列，每元素为一整数（长度）
class RingQueueOfInt
{
public:
    RingQueueOfInt(int max);
    ~RingQueueOfInt();

    //
    int Init();

    // 住队列中存入一个元素（不能存入，返回小于0值）
    int Push(void *element);

    // 取一个元素
    int Pop(void *&element);

    // 当前队列元素个数
    int Size();

    // 队列满返回true
    bool isFull();

    // 队列空返回true
    bool isEmpty();


private:
    static const int DEFAULT_COUNT_MAX = 500;
    const int m_max;

    void ** const m_buf;    // 数据存放处
    int m_read;             // 读位置
    int m_write;            // 写位置
};






}// end of RINGQUEUEOFINT_SPACE
using namespace RINGQUEUEOFINT_SPACE;
#endif // end of __RINGQUEUEOFINT_H_20100426__
