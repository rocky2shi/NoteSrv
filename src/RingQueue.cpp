// Rocky 2009-11-10 17:16:34
#include "RingQueue.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <new>
namespace RINGQUEUE_SPACE
{

#if _DEBUG
#define Debug(format, args...)   printf("%s:%d : " format "\n", __FILE__, __LINE__, ##args)
#else
#define Debug
#endif


/*
 *   环形队列数据结构。
 *
 */



// 环形队列默认长度
const int RingQueue::QUEUE_DEFAULT_LEN = 1024*1024*8;    // 8M 环形队列空间






RingQueue::RingQueue(int max/*=QUEUE_DEFAULT_LEN*/) :  m_MemSize(max)
{
}

RingQueue::~RingQueue()
{
    FreeMemory();
}

/*
 * 设置队列头数据初始值
 * 返回：成功返回0，出错返回非0。
 */
int RingQueue::Init()
{
    // 取内存
    if(GetMemory() < 0)
    {
        perror("Init()");
        return -1;
    }

    return 0;
}


/*
 * 显示内存（主要用于调试）
 */
void RingQueue::Display()
{
    Debug("读写位置：w:%u, r:%u, max:%u, mark:%u",
                m_head->nWriteSeekPos,
                m_head->nReadSeekPos,
                m_head->nMaxLen,
                m_head->nMarkPosOfEndWrite);
}

/*
 * 分配内存
 */
int RingQueue::GetMemory()
{
    m_head = (Head *)malloc(m_MemSize);
    return (NULL != m_head) ? 0 : -1;
}

/*
 * 和配套GetMemory()
 */
void RingQueue::FreeMemory()
{
    free(m_head);
    m_head = NULL;
}

// 重置内存区（头部等）
void RingQueue::Reset()
{
    assert(NULL != m_head);
    memset(m_head, 0, sizeof(Head));
    m_head->nMaxLen = m_MemSize - sizeof(Head);
}

// 取当前写位置
void *RingQueue::WritePos()
{
    assert(NULL != m_head);
    return (m_head->buf + m_head->nWriteSeekPos);
}

// 取当前读位置
void *RingQueue::ReadPos()
{
    assert(NULL != m_head);
    return (m_head->buf + m_head->nReadSeekPos);
}

// 内存空间满返true，否则返false；
bool RingQueue::isFull(unsigned int nDataLen)
{
    assert(NULL != m_head);

    // 数据长度还要加上单元头部长度
    const unsigned int nPackLen = nDataLen + sizeof(CellHead);

    /* 情况一：
     *      写 < 读 ，且剩余空间不足，则为满；
     */
    if( m_head->nReadSeekPos > m_head->nWriteSeekPos
        && m_head->nReadSeekPos - m_head->nWriteSeekPos <= nPackLen // 注是“<=”
      )
    {
        return true;
    }

    /* 情况二：
     *      读 < 写，且尾部空间不足，且（回绕到）开始处空间也不足，则为满；
     */
    if( m_head->nWriteSeekPos > m_head->nReadSeekPos
        && m_head->nMaxLen - m_head->nWriteSeekPos < nPackLen   //
        && m_head->nReadSeekPos <= nPackLen    // 注是“<=”
      )
    {
        return true;
    }

    return false;
}

// 内存空间无数据返true，否则返false；
bool RingQueue::isEmpty()
{
    assert(NULL != m_head);
    // （读、写处于同一位置时为空）
    return m_head->nReadSeekPos == m_head->nWriteSeekPos;
}

// 取当前处单元数据长度
int RingQueue::GetCurrentDataLen()
{
    // 取当前读位置
    CellHead *cell = (CellHead *)ReadPos();
    return cell->nBodyLen;
}

/*
 * 把pPackBuf放到队列。注意，长度在包头里指定。
 * 返回：成功返回0，出错返回非0。
 */
int RingQueue::PutPackToQueue(const void *pData, unsigned int nDataLen)
{
    // 数据长度还要加上单元头部长度
    const unsigned int nPackLen = nDataLen + sizeof(CellHead);

    // 检查剩余空间
    if( isFull(nPackLen) )
    {
        // Debug("空间满");
        return -1;  // 已无足够空间
    }

    /* 检查当前写的位置，有空（不满）的空间可能在数据区前，也可能在后；
     * 这种检查只有在写入数据时才能做测试，因为长度不确定；
     */
    if(m_head->nMaxLen - m_head->nWriteSeekPos < nPackLen)
    {
        m_head->nMarkPosOfEndWrite = m_head->nWriteSeekPos; // 记录回绕前，末尾处内数据位置；
        m_head->nWriteSeekPos = 0; // 移到数据区起始处
    }

    // 写的位置
    CellHead *cell = (CellHead *)WritePos();

    // 标明长度
    cell->nBodyLen = nDataLen;

    // 数据放到队列
    memcpy(cell->data, pData, nDataLen);

    // 移动写位置（数据长度 + 单元头长度）
    m_head->nWriteSeekPos += nPackLen;

    return 0;
}

/*
 * 从队列中取一个包，放到pPackBuf中。
 * 返回：成功返回0，出错返回非0。
 */
int RingQueue::GetPackFromQueue(void *pBuf, unsigned int nBufLen)
{
    if( isEmpty() )
    {
        // Debug("无数据");
        return -1;
    }

    // 查看上次读位置是否已处于数据末尾
    if(m_head->nReadSeekPos == m_head->nMarkPosOfEndWrite)
    {
        m_head->nReadSeekPos = 0;  // 移到数据区起始处
    }

    // 取当前读位置
    CellHead *cell = (CellHead *)ReadPos();

    // 检查长度
    if(nBufLen < cell->nBodyLen)
    {
        return -2; // 传入的缓冲区太小
    }

    // 复制数据
    memcpy(pBuf, cell->data, cell->nBodyLen);

    // 数据长度还要加上单元头部长度
    const unsigned int nPackLen = cell->nBodyLen + sizeof(CellHead);

    // 移动读位置
    m_head->nReadSeekPos += nPackLen;

    return 0;
}










}
