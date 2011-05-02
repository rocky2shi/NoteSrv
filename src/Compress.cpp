// Rocky 2010-05-10 10:53:52
#include "Compress.h"
namespace COMPRESS_SPACE
{




Compress::Compress()
{
}

Compress::~Compress()
{
}

// ���ʼ��
int Compress::init()
{
    int ret;
    static Compress executor; // ִ�о��崦��Ķ���ʵ��

    Compress *obj = Compress::instance( &executor );
    if(NULL == obj)
    {
        printf("Compress::instance() error\n");
        return ERR;
    }

    return OK;
}

// �ڴ�ѹ�����ɹ�����OK��
int Compress::Do(const void *pSrc, int nSrcLen, void *pDest, int nDestLen)
{
    assert(NULL != pSrc && NULL != pDest);
    memcpy(pDest, pSrc, nSrcLen);
    return OK;
}

// �ڴ��ѹ�����ɹ�����OK��
int Compress::Undo(const void *pSrc, int nSrcLen, void *pDest, int nDestLen)
{
    assert(NULL != pSrc && NULL != pDest);
    memcpy(pDest, pSrc, nSrcLen);
    return OK;
}

// �ļ�ѹ��
int Compress::Do(const FileObj &src, FileObj &dest)
{
    const int len = src.Size();
    auto_ptr<char> buf( new char[len + 1]);
    if(buf.get() == NULL)
    {
        LOG_ERROR("new char[%d] error.", len + 1);
        return ERR;
    }
    src.Read(buf.get(), len);
    dest.Write(buf.get(), len);
    return OK;
}

// �Ľ��ѹ��
int Compress::Undo(const FileObj &src, FileObj &dest)
{
    return Do(src, dest);
}







}// end of COMPRESS_SPACE
