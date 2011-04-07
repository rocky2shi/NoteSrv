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

// 类初始化
int Compress::init()
{
    int ret;
    static Compress executor; // 执行具体处理的对象实例

    Compress *obj = Compress::instance( &executor );
    if(NULL == obj)
    {
        printf("Compress::instance() error\n");
        return ERR;
    }

    return OK;
}

// 内存压缩（成功返回OK）
int Compress::Do(const void *pSrc, int nSrcLen, void *pDest, int nDestLen)
{
    assert(NULL != pSrc && NULL != pDest);
    memcpy(pDest, pSrc, nSrcLen);
    return OK;
}

// 内存解压缩（成功返回OK）
int Compress::Undo(const void *pSrc, int nSrcLen, void *pDest, int nDestLen)
{
    assert(NULL != pSrc && NULL != pDest);
    memcpy(pDest, pSrc, nSrcLen);
    return OK;
}

// 文件压缩
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

// 文解件压缩
int Compress::Undo(const FileObj &src, FileObj &dest)
{
    return Do(src, dest);
}







}// end of COMPRESS_SPACE
