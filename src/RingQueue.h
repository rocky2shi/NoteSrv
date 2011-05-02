#ifndef __RINGQUEUE_H_20091110__
#define __RINGQUEUE_H_20091110__
namespace RINGQUEUE_SPACE
{




class RingQueue
{
    // ����ͷ��
    struct Head
    {
        // ��λ�ã��Ӹ�λ�ö�һ�����ݰ�����������ڴ�����ƫ�����ֽ�������ͬ��
        unsigned int nReadSeekPos;
        // дλ�ã��Ӹ�λ�ÿ�ʼд
        unsigned int nWriteSeekPos;
        // ����ڴ���ĩβдλ�ã���ִ�ж�����ʱ�����Ѵ��ڸ�λ�ã�Ӧ���ƣ���������ʼ�������
        unsigned int nMarkPosOfEndWrite;
        // ���ݲ����ڴ泤��
        unsigned int nMaxLen;
        // ���ݲ��֣�ռλ����
        char buf[0];
    };

    // ÿ����Ԫ��ͷ��
    struct CellHead
    {
        unsigned short int nBodyLen; // �����峤��
        char data[0]; // ����Ϊ0��ռλ��
    };


public:
    RingQueue(int max=QUEUE_DEFAULT_LEN);
    ~RingQueue();

    // ���ö���ͷ���ݳ�ʼֵ
    // ���أ��ɹ�����0�������ط�0��
    int Init();

    // �Ƿ�Ϊ��
    bool isEmpty();

    // �Ӷ�����ȡһ��������ͷ�Ͱ��壩���ŵ�pPackBuf�С�
    // ���أ��ɹ�����0�������ط�0��
    int GetPackFromQueue(void *pBuf, unsigned int nBufLen);

    // ��pPackBuf�ŵ����С�ע�⣬�����ڰ�ͷ��ָ����
    // ���أ��ɹ�����0�������ط�0��
    int PutPackToQueue(const void *pData, unsigned int nDataLen);

    // ȡ��ǰ����Ԫ���ݳ���
    int GetCurrentDataLen();

    void Display();

private:
    // ���䱾���ڴ棬��ϵͳ�����ڴ棻
    int GetMemory();
    // ������GetMemory()
    void FreeMemory();
    // ȡ��ǰ����дλ��
    void *WritePos();
    void *ReadPos();

    void Reset();
    bool isFull(unsigned int nDataLen);



private:
    Head *m_head;
    unsigned int m_MemSize;   // �ڴ��С��ͷ���������壩
    static const int QUEUE_DEFAULT_LEN;
};







}
using namespace RINGQUEUE_SPACE;
#endif // end of __RINGQUEUE_H_20091110__
