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
 *   ���ζ������ݽṹ��
 *
 */



// ���ζ���Ĭ�ϳ���
const int RingQueue::QUEUE_DEFAULT_LEN = 1024*1024*8;    // 8M ���ζ��пռ�






RingQueue::RingQueue(int max/*=QUEUE_DEFAULT_LEN*/) :  m_MemSize(max)
{
}

RingQueue::~RingQueue()
{
    FreeMemory();
}

/*
 * ���ö���ͷ���ݳ�ʼֵ
 * ���أ��ɹ�����0�������ط�0��
 */
int RingQueue::Init()
{
    // ȡ�ڴ�
    if(GetMemory() < 0)
    {
        perror("Init()");
        return -1;
    }

    return 0;
}


/*
 * ��ʾ�ڴ棨��Ҫ���ڵ��ԣ�
 */
void RingQueue::Display()
{
    Debug("��дλ�ã�w:%u, r:%u, max:%u, mark:%u",
                m_head->nWriteSeekPos,
                m_head->nReadSeekPos,
                m_head->nMaxLen,
                m_head->nMarkPosOfEndWrite);
}

/*
 * �����ڴ�
 */
int RingQueue::GetMemory()
{
    m_head = (Head *)malloc(m_MemSize);
    return (NULL != m_head) ? 0 : -1;
}

/*
 * ������GetMemory()
 */
void RingQueue::FreeMemory()
{
    free(m_head);
    m_head = NULL;
}

// �����ڴ�����ͷ���ȣ�
void RingQueue::Reset()
{
    assert(NULL != m_head);
    memset(m_head, 0, sizeof(Head));
    m_head->nMaxLen = m_MemSize - sizeof(Head);
}

// ȡ��ǰдλ��
void *RingQueue::WritePos()
{
    assert(NULL != m_head);
    return (m_head->buf + m_head->nWriteSeekPos);
}

// ȡ��ǰ��λ��
void *RingQueue::ReadPos()
{
    assert(NULL != m_head);
    return (m_head->buf + m_head->nReadSeekPos);
}

// �ڴ�ռ�����true������false��
bool RingQueue::isFull(unsigned int nDataLen)
{
    assert(NULL != m_head);

    // ���ݳ��Ȼ�Ҫ���ϵ�Ԫͷ������
    const unsigned int nPackLen = nDataLen + sizeof(CellHead);

    /* ���һ��
     *      д < �� ����ʣ��ռ䲻�㣬��Ϊ����
     */
    if( m_head->nReadSeekPos > m_head->nWriteSeekPos
        && m_head->nReadSeekPos - m_head->nWriteSeekPos <= nPackLen // ע�ǡ�<=��
      )
    {
        return true;
    }

    /* �������
     *      �� < д����β���ռ䲻�㣬�ң����Ƶ�����ʼ���ռ�Ҳ���㣬��Ϊ����
     */
    if( m_head->nWriteSeekPos > m_head->nReadSeekPos
        && m_head->nMaxLen - m_head->nWriteSeekPos < nPackLen   //
        && m_head->nReadSeekPos <= nPackLen    // ע�ǡ�<=��
      )
    {
        return true;
    }

    return false;
}

// �ڴ�ռ������ݷ�true������false��
bool RingQueue::isEmpty()
{
    assert(NULL != m_head);
    // ������д����ͬһλ��ʱΪ�գ�
    return m_head->nReadSeekPos == m_head->nWriteSeekPos;
}

// ȡ��ǰ����Ԫ���ݳ���
int RingQueue::GetCurrentDataLen()
{
    // ȡ��ǰ��λ��
    CellHead *cell = (CellHead *)ReadPos();
    return cell->nBodyLen;
}

/*
 * ��pPackBuf�ŵ����С�ע�⣬�����ڰ�ͷ��ָ����
 * ���أ��ɹ�����0�������ط�0��
 */
int RingQueue::PutPackToQueue(const void *pData, unsigned int nDataLen)
{
    // ���ݳ��Ȼ�Ҫ���ϵ�Ԫͷ������
    const unsigned int nPackLen = nDataLen + sizeof(CellHead);

    // ���ʣ��ռ�
    if( isFull(nPackLen) )
    {
        // Debug("�ռ���");
        return -1;  // �����㹻�ռ�
    }

    /* ��鵱ǰд��λ�ã��пգ��������Ŀռ������������ǰ��Ҳ�����ں�
     * ���ּ��ֻ����д������ʱ���������ԣ���Ϊ���Ȳ�ȷ����
     */
    if(m_head->nMaxLen - m_head->nWriteSeekPos < nPackLen)
    {
        m_head->nMarkPosOfEndWrite = m_head->nWriteSeekPos; // ��¼����ǰ��ĩβ��������λ�ã�
        m_head->nWriteSeekPos = 0; // �Ƶ���������ʼ��
    }

    // д��λ��
    CellHead *cell = (CellHead *)WritePos();

    // ��������
    cell->nBodyLen = nDataLen;

    // ���ݷŵ�����
    memcpy(cell->data, pData, nDataLen);

    // �ƶ�дλ�ã����ݳ��� + ��Ԫͷ���ȣ�
    m_head->nWriteSeekPos += nPackLen;

    return 0;
}

/*
 * �Ӷ�����ȡһ�������ŵ�pPackBuf�С�
 * ���أ��ɹ�����0�������ط�0��
 */
int RingQueue::GetPackFromQueue(void *pBuf, unsigned int nBufLen)
{
    if( isEmpty() )
    {
        // Debug("������");
        return -1;
    }

    // �鿴�ϴζ�λ���Ƿ��Ѵ�������ĩβ
    if(m_head->nReadSeekPos == m_head->nMarkPosOfEndWrite)
    {
        m_head->nReadSeekPos = 0;  // �Ƶ���������ʼ��
    }

    // ȡ��ǰ��λ��
    CellHead *cell = (CellHead *)ReadPos();

    // ��鳤��
    if(nBufLen < cell->nBodyLen)
    {
        return -2; // ����Ļ�����̫С
    }

    // ��������
    memcpy(pBuf, cell->data, cell->nBodyLen);

    // ���ݳ��Ȼ�Ҫ���ϵ�Ԫͷ������
    const unsigned int nPackLen = cell->nBodyLen + sizeof(CellHead);

    // �ƶ���λ��
    m_head->nReadSeekPos += nPackLen;

    return 0;
}










}
