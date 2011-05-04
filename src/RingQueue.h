#ifndef __RINGQUEUE_H_20091110__
#define __RINGQUEUE_H_20091110__
namespace RINGQUEUE_SPACE
{




class RingQueue
{
    // 队列头部
    struct Head
    {
        // 读位置，从该位置读一个数据包（相对整个内存区的偏量，字节数，下同）
        unsigned int nReadSeekPos;
        // 写位置，从该位置开始写
        unsigned int nWriteSeekPos;
        // 标记内存区末尾写位置，当执行读操作时，若已处于该位置，应回绕，从数据起始处理读；
        unsigned int nMarkPosOfEndWrite;
        // 数据部分内存长度
        unsigned int nMaxLen;
        // 数据部分（占位符）
        char buf[0];
    };

    // 每个单元的头部
    struct CellHead
    {
        unsigned short int nBodyLen; // 数据体长度
        char data[0]; // 长度为0的占位符
    };


public:
    RingQueue(int max=QUEUE_DEFAULT_LEN);
    ~RingQueue();

    // 设置队列头数据初始值
    // 返回：成功返回0，出错返回非0。
    int Init();

    // 是否为空
    bool isEmpty();

    // 从队列中取一个包（包头和包体），放到pPackBuf中。
    // 返回：成功返回0，出错返回非0。
    int GetPackFromQueue(void *pBuf, unsigned int nBufLen);

    // 把pPackBuf放到队列。注意，长度在包头里指定。
    // 返回：成功返回0，出错返回非0。
    int PutPackToQueue(const void *pData, unsigned int nDataLen);

    // 取当前处单元数据长度
    int GetCurrentDataLen();

    void Display();

private:
    // 分配本地内存，或系统共享内存；
    int GetMemory();
    // 和配套GetMemory()
    void FreeMemory();
    // 取当前读、写位置
    void *WritePos();
    void *ReadPos();

    void Reset();
    bool isFull(unsigned int nDataLen);



private:
    Head *m_head;
    unsigned int m_MemSize;   // 内存大小（头部＋数据体）
    static const int QUEUE_DEFAULT_LEN;
};







}
using namespace RINGQUEUE_SPACE;
#endif // end of __RINGQUEUE_H_20091110__
